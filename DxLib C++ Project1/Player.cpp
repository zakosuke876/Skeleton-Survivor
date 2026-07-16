#include "Player.h"
#include "Sword.h"
#include "Ground.h"
#include "GameConfig.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "PlayerDataLoader.h"
#include "DxLib.h"
#include <cmath>
#include <algorithm>

const PlayerAnimData PLAYER_ANIM_TABLE[] = {

// アニメーション番号, アニメーション再生スピード, ループ設定

	{12, 0.15f, false},
	{13, 0.2f, false},
	{9, 0.1f, true},
	{15, 0.3f, true},
	{11, 0.4f, true},
	{7,  0.3f, false},
	{2,  0.3f, false},
	{5,  0.3f, false},
	{6,  0.3f, false},
};

Player::Player() {

	if (!PlayerDataLoader::Load((playerDataTable)))
	{
		printfDx("プレイヤーデータの読み込みに失敗しました\n");
	}

	// プレイヤーモデルを読み込む
	modelHandle = MV1LoadModel(playerDataTable.modelPath.c_str());

	//テクスチャ反映
	playerTexHandle = LoadGraph(playerDataTable.texturePath.c_str());
	MV1SetTextureGraphHandle(modelHandle, 0, playerTexHandle, FALSE);

	// モデルを拡大
	scale = playerDataTable.scale;
	MV1SetScale(modelHandle, VGet(scale, scale, scale));

	// 初期座標にセット
	MV1SetPosition(modelHandle, VGet(x, y, z));

	// 初期値設定
	Reset();
	
	sword.SetUpBone(modelHandle);
}

void Player::Reset() {

	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	angleY = 0.0f;

	// JSONファイルから読み込んだデータを代入
	hp = playerDataTable.maxHp;

	maxHp = playerDataTable.maxHp;

	attackPower = playerDataTable.attackPower;

	speed = playerDataTable.speed;

	dashMultiplier = playerDataTable.dashMultiplier;

	level = playerDataTable.startLevel;

	nextLevelExp = playerDataTable.startNextLevelExp;

	attackRadius = playerDataTable.attackRadius;

	bodyRadius = playerDataTable.bodyRadius;

	levelUpAttackPower = playerDataTable.levelUpAttackPower;

	nextLevelExpMultiplier = playerDataTable.nextLevelExpMultiplier;

	invincibleTime = playerDataTable.invincibleTime;

	hideTime = playerDataTable.hideTime;

	levelUpTextActiveTime = playerDataTable.levelUpTextActiveTime;

	exp = 0;
	isLevelUpTextActive = false;
	levelupTextActiveTimer = 0.0f;

	invincibleTimer = 0.0f;
	deathTimer = 0.0f;

	isActive = true;
	isDead = false;

	// 最初はスポーン状態
	state = PLAYER_SPAWN;

	int anim = rand() % 2;

	// 2種類のアニメーションからランダムに選ぶ
	if (anim == 0)
	{
		AnimSwitch(PLAYER_ANIM_SPAWN_AIR);
	}
	else
	{
		AnimSwitch(PLAYER_ANIM_SPAWN_GROUND);
	}
}

void Player::Update(float deltaTime) {

	// 死亡した場合処理しない
	if (!isActive) return;

	switch (state)
	{
	case PLAYER_SPAWN:

		UpdateSpawn();

		break;

	case PLAYER_IDLE:

		UpdateIdle();

		break;

	case PLAYER_WALK:

		UpdateWalk();

		break;

	case PLAYER_DASH:

		UpdateDash();

		break;

	case PLAYER_ATTACK:

		UpdateAttack();

		break;

	case PLAYER_DAMAGE:

		UpdateDamage();

		break;

	case PLAYER_DEATH:

		UpdateDeath(deltaTime);

		break;
	}

	// 無敵時間計測
	if (isInvincible)
	{
		invincibleTimer += deltaTime;

		// 無敵時間終了時にタイマーをリセット
		if (invincibleTimer >= invincibleTime)
		{
			isInvincible = false;
			invincibleTimer = 0.0f;
		}
	}

	// 再生時間を進める
	animTime += PLAYER_ANIM_TABLE[currentAnim].animSpeed;

	// アニメーション総再生時間を超えた場合
	if (animTime >= totalTime)
	{
		// ループ設定がTrueの場合
		if (PLAYER_ANIM_TABLE[currentAnim].loop)
		{
			// タイマーリセットしてループ
			animTime = 0.0f;
		}
		else
		{
			// 再生を停止
			animTime = totalTime;
		}
	}

	// レベルアップ中の場合のみ処理する
	if (isLevelUpTextActive)
	{
		levelupTextActiveTimer += deltaTime;

		if (levelupTextActiveTimer >= levelUpTextActiveTime)
		{
			levelupTextActiveTimer = 0.0f;
			isLevelUpTextActive = false;
		}
	}

	// 移動範囲を制限
	ClampPosition();

	// 時刻をセット
	MV1SetAttachAnimTime(modelHandle, attachIndex, animTime);

	// キャラの移動
	MV1SetPosition(modelHandle, VGet(x, y, z));

	// 向きのセット
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, angleY, 0.0f));

	sword.Update(modelHandle);
}

void Player::TakeDamage(int value, SoundManager& soundManager) {

	// 攻撃判定を無効
	attackHitBox.isAttackActive = false;

	// 無敵中なら処理しない
	if (isInvincible) return;

	// 死亡しているなら処理しない
	if (isDead) return;

	// 効果音再生を依頼(引数でタイプを渡す)
	soundManager.PlaySe(ATTACK_HIT);

	// 被弾アニメーション設定
	AnimSwitch(PLAYER_ANIM_DAMAGE);

	// 無敵時間開始
	isInvincible = true;

	state = PLAYER_DAMAGE;

	hp -= value;

	// HPの値を制限
	hp = std::clamp(hp, 0, maxHp);

	// 死亡判定
	if (hp <= 0)
	{
		Die();
	}
}

void Player::Recovery(int value) {

	hp += value;

	// HPを範囲内の値にする(0～maxHp)
	hp = std::clamp(hp, 0, maxHp);
}

void Player::AddExp(int value, const SoundManager& soundManager) {

	exp += value;

	// 保持している経験値量がレベルアップに必要な量を上回っている間ループさせる
	while (exp >= nextLevelExp)
	{
		// 余った経験値を次のレベルに引き継ぐ
		exp -= nextLevelExp;
		LevelUp(soundManager);
	}
}

void Player::LevelUp(const SoundManager& soundManager) {

	// 効果音再生を依頼(引数でタイプを渡す)
	soundManager.PlaySe(LEVELUP);

	level += 1;

	// レベルが上がるたびに攻撃力上昇
	attackPower += levelUpAttackPower;

	// 次のレベルアップにかかる経験値量を1.3倍にする
	nextLevelExp = static_cast<int>(nextLevelExp * nextLevelExpMultiplier);
	isLevelUpTextActive = true;
	levelupTextActiveTimer = 0.0f;
}

void Player::Draw() const {

	if (!isActive) return;

	// モデルを描画
	MV1DrawModel(modelHandle);

	sword.Draw();
}

void Player::DrawUI(const FontManager& fontManager) const {

	if (!isActive) return;

	DrawHpGauge(fontManager);

	DrawExpGauge(fontManager);

	// レベルアップ中のみテキストを表示する
	if (!isLevelUpTextActive) return;

	// レベルアップテキストの表示制御
	VECTOR posTex = VGet(x, y, z);
	posTex.x -= 30.0f;
	posTex.y += 40.0f;

	// ワールド座標をスクリーン座標に変換
	VECTOR screenPosTex = ConvWorldPosToScreenPos(posTex);

	DrawStringToHandle(screenPosTex.x, screenPosTex.y, "LEVEL UP!!", GetColor(255, 0, 0), fontManager.GetPlayerUIFont());
}

// スポーン状態
void Player::UpdateSpawn() {

	// スポーン完了後は待機状態へ遷移
	if (animTime >= totalTime)
	{
		state = PLAYER_IDLE;
	}
}

// 待機状態
void Player::UpdateIdle() {

	// 入力が無い場合待機アニメーション再生
	AnimSwitch(PLAYER_ANIM_IDLE);

	// 攻撃が最優先
	// 攻撃入力があれば攻撃状態へ
	if (IsAttackInput())
	{
		state = PLAYER_ATTACK;
	}
	else if (IsMoveInput()) // 移動入力があった場合
	{
		// 移動入力中かつダッシュ入力中ならダッシュ状態へ
		if (IsDashInput())
		{
			state = PLAYER_DASH;
		}
		else
		{
			state = PLAYER_WALK;
		}
	}
}

void Player::UpdateWalk() {

	AnimSwitch(PLAYER_ANIM_WALK);

	Move(speed);

	// 移動入力がなくなった場合
	if (!IsMoveInput())
	{
		state = PLAYER_IDLE;
	}

	// ダッシュ入力があった場合
	if (IsDashInput())
	{
		state = PLAYER_DASH;
	}

	// 攻撃入力があった場合
	if (IsAttackInput())
	{
		state = PLAYER_ATTACK;
	}
}

void Player::UpdateDash() {

	AnimSwitch(PLAYER_ANIM_DASH);

	// ダッシュ時はスピード2.5倍
	Move(speed * dashMultiplier);

	if (!IsMoveInput())
	{
		state = PLAYER_IDLE;
	}

	if (!IsDashInput())
	{
		state = PLAYER_WALK;
	}

	if (IsAttackInput())
	{
		state = PLAYER_ATTACK;
	}
}

void Player::UpdateAttack() {

	// 攻撃判定を無効
	attackHitBox.isAttackActive = false;

	AnimSwitch(PLAYER_ANIM_ATTACK);

	if (currentAnim == PLAYER_ANIM_ATTACK)
	{
		// 当たり判定が発生するのは10fから18fまで
		bool isAttackHitActive = (animTime >= ATTACK_HIT_START_FRAME && animTime <= ATTACK_HIT_END_FRAME);

		// 構造体変数に代入
		attackHitBox.x = sword.GetPos().x;
		attackHitBox.z = sword.GetPos().z;
		attackHitBox.radius = attackRadius;
		attackHitBox.isAttackActive = isAttackHitActive;
	}

	// 攻撃アニメーション終了後は待機状態へ遷移
	if (animTime >= totalTime)
	{
		// 攻撃判定を無効
		attackHitBox.isAttackActive = false;
		state = PLAYER_IDLE;
	}
}

void Player::UpdateDamage() {

	// 攻撃判定を無効
	attackHitBox.isAttackActive = false;

	AnimSwitch(PLAYER_ANIM_DAMAGE);

	// ダメージアニメーション終了後は待機状態へ遷移
	if (animTime >= totalTime)
	{
		state = PLAYER_IDLE;
	}
}

void Player::UpdateDeath(float deltaTime) {

	// 攻撃判定を無効
	attackHitBox.isAttackActive = false;

	if (animTime >= totalTime)
	{
		AnimSwitch(PLAYER_ANIM_DEATHPOSE);
	}

	deathTimer += deltaTime;

	if (deathTimer >= hideTime)
	{
		// プレイヤーを無効化
		deathTimer = 0.0f;
		isActive = false;
	}
}

void Player::Move(float moveSpeed) {

	float moveX = 0.0f;
	float moveZ = 0.0f;

	// キー入力
	if (CheckHitKey(KEY_INPUT_W)) moveZ += 1.0f;
	if (CheckHitKey(KEY_INPUT_S)) moveZ -= 1.0f;
	if (CheckHitKey(KEY_INPUT_A)) moveX -= 1.0f;
	if (CheckHitKey(KEY_INPUT_D)) moveX += 1.0f;

	float length = sqrtf(moveX * moveX + moveZ * moveZ);

	// 0除算対策
	if (length > 0.0f)
	{
		// 正規化
		moveX /= length;
		moveZ /= length;

		x += moveX * moveSpeed;
		z += moveZ * moveSpeed;

		// 移動方向に向ける
		float angle = atan2f(moveX, moveZ) + DX_PI_F;

		angleY = angle;
	}
}

void Player::ClampPosition() {

	// 移動範囲の制限
	if (x < Ground::GetMinXPos()) x = Ground::GetMinXPos();
	if (x > Ground::GetMaxXPos())  x = Ground::GetMaxXPos();
	if (z < Ground::GetMinZPos()) z = Ground::GetMinZPos();
	if (z > Ground::GetMaxZPos())  z = Ground::GetMaxZPos();
}

bool Player::IsMoveInput() {

	return CheckHitKey(KEY_INPUT_W) ||
		   CheckHitKey(KEY_INPUT_S) ||
		   CheckHitKey(KEY_INPUT_A) ||
		   CheckHitKey(KEY_INPUT_D);
}

bool Player::IsDashInput() {

	return CheckHitKey(KEY_INPUT_LSHIFT) ||
		   CheckHitKey(KEY_INPUT_RSHIFT);
}

bool Player::IsAttackInput() {

	return CheckHitKey(KEY_INPUT_SPACE);
}

void Player::DrawHpGauge(const FontManager& fontManager) const {

	// 現在HPの割合(0.0～1.0)
	const float hpRate = static_cast<float>(hp) / maxHp;
	int color = GetColor(0, 255, 0);

	// HPが半分以下でゲージを黄色にする
	if (hpRate <= HP_GAUGE_YELLOW_VALUE)
	{
		color = GetColor(255, 255, 0);
	}

	int gaugeLeft = PLAYER_HPGAUGE_X;
	int gaugeTop = PLAYER_HPGAUGE_Y;
	int gaugeRight = PLAYER_HPGAUGE_X + HP_GAUGE_WIDTH;
	int gaugeBottom = PLAYER_HPGAUGE_Y + PLAYER_HPGAUGE_BOTTOM;

	// HPゲージ
	// HPゲージ背景(赤表示)
	DrawBox(gaugeLeft, gaugeTop, gaugeRight, gaugeBottom, GetColor(255, 0, 0), TRUE);

	// 現在HP(緑表示・ダメージを受けると背景の赤が見えるようになる)
	DrawBox(gaugeLeft, gaugeTop, gaugeLeft + static_cast<int>(HP_GAUGE_WIDTH * hpRate), gaugeBottom, color, TRUE);

	// HPゲージの枠(白表示)
	DrawLineBox(gaugeLeft, gaugeTop, gaugeRight, gaugeBottom, GetColor(255, 255, 255));

	// HPテキストの表示
	DrawFormatStringToHandle(40, gaugeTop, GetColor(255, 0, 0), fontManager.GetPlayerUIFont(), "HP");
}

void Player::DrawExpGauge(const FontManager& fontManager) const {

	// 経験値ゲージ
	const float expRate = static_cast<float>(exp) / nextLevelExp;
	int expGaugeLeft = PLAYER_HPGAUGE_X;

	// HPゲージの下に配置
	int expGaugeTop = PLAYER_HPGAUGE_Y + EXP_GAUGE_OFFSET_Y;
	int expGaugeRight = PLAYER_HPGAUGE_X + HP_GAUGE_WIDTH;
	int expGaugeBottom = expGaugeTop + PLAYER_HPGAUGE_BOTTOM;

	// 背景(黒)
	DrawBox(expGaugeLeft, expGaugeTop, expGaugeRight, expGaugeBottom, GetColor(0, 0, 0), TRUE);
	// 現在経験値(青)
	DrawBox(expGaugeLeft, expGaugeTop, expGaugeLeft + static_cast<int>(HP_GAUGE_WIDTH * expRate), expGaugeBottom, GetColor(0, 100, 255), TRUE);
	// 枠線(白)
	DrawLineBox(expGaugeLeft, expGaugeTop, expGaugeRight, expGaugeBottom, GetColor(255, 255, 255));

	// 現在のLVを表示
	DrawFormatStringToHandle(LV_TEXT_X, expGaugeTop + LV_TEXT_OFFSET_Y, GetColor(255, 0, 0), fontManager.GetPlayerUIFont(), "Lv:%d", level);
}


void Player::AnimSwitch(int animNum) {

	// 違うアニメーションなら切り替える
	if (currentAnim != animNum)
	{
		// 現在のアニメーションを削除
		MV1DetachAnim(modelHandle, attachIndex);

		// アニメーションをアタッチ
		attachIndex = MV1AttachAnim(modelHandle, PLAYER_ANIM_TABLE[animNum].animIndex);

		// 総再生時間を取得
		totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);

		// 現在のアニメーション番号を保持
		currentAnim = animNum;

		// 再生タイマーリセット
		animTime = 0.0f;
	}
}

void Player::Die() {

	hp = 0;
	isDead = true;
	AnimSwitch(PLAYER_ANIM_DEATH);
	state = PLAYER_DEATH;
}