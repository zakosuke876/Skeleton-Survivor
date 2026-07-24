#include "Enemy.h"
#include "Player.h"
#include "GameConfig.h"
#include "SoundManager.h"


#include "MagicEffectManager.h"

Enemy::Enemy() {

	// ランダム生成用
	std::random_device rd;
	rng = std::mt19937(rd());

	// 初期化
	isActive = true;
	hasTargetPosition = false;

	// 無敵時間関係
	isInvincible = false;
	invincibleTimer = 0.0f;
}

Enemy::~Enemy() {

	// リソースの開放
	MV1DeleteModel(modelHandle);
}

void Enemy::SwitchAnimation(int animationNum) {

	// 同じアニメーションの場合処理しない
	if (currentAnim == animationNum) return;

	MV1DetachAnim(modelHandle, attachIndex);

	// animTableを使い、子クラスごとの異なるアニメーション番号を取得する
	attachIndex = MV1AttachAnim(modelHandle, animTable[animationNum].animIndex);

	totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
	animTime = 0.0f;
	currentAnim = animationNum;
}

void Enemy::Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) {

	// アニメーション再生タイマーを進める
	UpdateAnim();

	// アニメーションの再生時間を反映
	MV1SetAttachAnimTime(modelHandle, attachIndex, animTime);

	// 無敵時間計測
	if (isInvincible)
	{
		invincibleTimer += deltaTime;

		if (invincibleTimer >= invincibleTime)
		{
			invincibleTimer = 0.0f;
			isInvincible = false;
		}
	}
}

void Enemy::UpdatePatrol(const Player& player) {

	if (!hasTargetPosition)
	{
		// 目的地X,Z座標の範囲設定
		std::uniform_real_distribution<float> distanceX(GameConfig::FIELD_MIN_X, GameConfig::FIELD_MAX_X);
		std::uniform_real_distribution<float> distanceZ(GameConfig::FIELD_MIN_Z, GameConfig::FIELD_MAX_Z);

		targetX = distanceX(rng);
		targetZ = distanceZ(rng);

		// 目的地到着まで再計算しない
		hasTargetPosition = true;
	}

	// 到着判定に使うため、移動前の目的地までの距離を受け取る
	float distance = MoveToward(targetX, targetZ, speed);

	if (distance <= 5.0f)
	{
		// 目的地を再計算
		hasTargetPosition = false;
	}

	// プレイヤーが追跡範囲内に入ったか
	if (IsPlayerInChaseRange(player))
	{
		state = ENEMY_CHASE;
		SwitchAnimation(ENEMY_ANIM_DASH);
	}

	MV1SetPosition(modelHandle, VGet(x, y, z));
}

void Enemy::UpdateChase(const Player& player) {

	// プレイヤーが追跡範囲外に出たか
	if (!IsPlayerInChaseRange(player))
	{
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}

	// プレイヤーが攻撃範囲内に入ったか
	if (IsPlayerInAttackRange(player))
	{
		state = ENEMY_ATTACK;
		SwitchAnimation(ENEMY_ANIM_ATTACK);
	}

	MoveToward(player.GetPosition().x, player.GetPosition().z, dashSpeed);

	MV1SetPosition(modelHandle, VGet(x, y, z));
}

float Enemy::MoveToward(float targetX, float targetZ, float speed) {

	float dx = targetX - x;
	float dz = targetZ - z;

	float distance = sqrt(dx * dx + dz * dz);

	if (distance > 0.0f)
	{
		float dirX = dx / distance;
		float dirZ = dz / distance;

		angleY = atan2f(dx, dz) + DX_PI_F;
		MV1SetRotationXYZ(modelHandle, VGet(0, angleY, 0));

		x += dirX * speed;
		z += dirZ * speed;
	}

	return distance;
}

void Enemy::UpdateAnim() {

	// アニメーションごとに設定した時間分進める
	animTime += animTable[currentAnim].animSpeed;

	// 総再生時間を超えたらループ
	if (animTime >= totalTime)
	{
		// アニメーションをループさせるか
		if (animTable[currentAnim].loop)
		{
			// 再生時間をリセットしてループ
			animTime = 0.0f;
		}
		else
		{
			// 再生を停止
			animTime = totalTime;
		}
	}
}

// ダメージを与え、死亡した場合はtrueを返す
bool Enemy::TakeDamage(int value, SoundManager& soundManager) {
	
	// 無敵時間中は処理しない
	if (isInvincible) return false;

	// スポーン中は処理しない
	if (state == ENEMY_SPAWN) return false;

	// 効果音再生を依頼(引数でタイプを渡す)
	soundManager.PlaySe(ATTACK_HIT);

	SwitchAnimation(ENEMY_ANIM_DAMAGE);

	// 無敵時間開始
	isInvincible = true;

	state = ENEMY_DAMAGE;

	hp -= value;
	
	// 死亡した場合
	if (hp <= 0)
	{
		Die();

		// 死亡したことを伝える
		return true;
	}

	return false;
}

void Enemy::Die() {

	deathTimer = 0.0f;
	hp = 0;
	SwitchAnimation(ENEMY_ANIM_DEATH);
	state = ENEMY_DEATH;
}

void Enemy::Draw() const {

	if (!isActive) return;

	DrawModel();
}

void Enemy::DrawUI() const {

	if (!isActive) return;

	// HPが最大値の場合は描画しない
	if (hp >= maxHp) return;


	VECTOR pos = VGet(x, y, z);
	pos.y += ENEMY_HPGAUGE_WORLD_OFFSET_Y;

	// ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(pos);

	// 現在HPの割合(0.0～1.0)
	const float hpRate = static_cast<float>(hp) / maxHp;

	int color = GetColor(0, 255, 0);

	// HPが半分以下でゲージを黄色にする
	if (hpRate <= HP_GAUGE_YELLOW_VALUE)
	{
		color = GetColor(255, 255, 0);
	}

	int gaugeLeft = screenPos.x + ENEMY_HPGAUGE_OFFSET_X;
	int gaugeTop = screenPos.y + ENEMY_HPGAUGE_OFFSET_Y;
	int gaugeRight  = screenPos.x + ENEMY_HPGAUGE_OFFSET_X + hpGaugeWidth;
	int gaugeBottom = screenPos.y;

	// HPゲージ
	// 背景(赤)
	DrawBox(gaugeLeft, gaugeTop, gaugeRight, gaugeBottom, GetColor(255, 0, 0), TRUE);

	// 現在HP(緑)
	DrawBox(gaugeLeft, gaugeTop, gaugeLeft + int(hpGaugeWidth * hpRate), gaugeBottom, color, TRUE);

	// 枠線(白)
	DrawLineBox(gaugeLeft, gaugeTop, gaugeRight, gaugeBottom, GetColor(255, 255, 255));
}

bool Enemy::IsPlayerInChaseRange(const Player& player) const {

	// 自分とプレイヤーの距離が±chaseRangeRadius未満の場合CHASE状態へ
	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	return (dx * dx + dz * dz) < (chaseRangeRadius * chaseRangeRadius);
}

bool Enemy::IsPlayerInAttackRange(const Player& player) const {

	// // 自分とプレイヤーの距離が±attackRangeRadius未満の場合CHASE状態へ[
	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	return (dx * dx + dz * dz) < (attackRangeRadius * attackRangeRadius);
}
