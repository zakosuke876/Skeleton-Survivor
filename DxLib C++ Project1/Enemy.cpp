#include "Enemy.h"
#include "Player.h"
#include "SoundManager.h"

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

void Enemy::Update(Player& player, float deltaTime) {

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
