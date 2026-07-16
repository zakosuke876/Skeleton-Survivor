#include "Skeleton_Normal.h"
#include "Player.h"
#include "EnemyNormalDataLoader.h"
#include "GameConfig.h"
#include "DxLib.h"


const EnemyAnimData SKELETON_NORMAL_ANIM_TABLE[] = {

	// SKELETON_NORMAL専用のアニメーションテーブル
	// EnemyAnimationのenum順と一致させる
	{7, 0.3f, false},   // ENEMY_ANIM_SPAWN
	{5, 0.3f, true},   // ENEMY_ANIM_IDLE
	{9, 0.3f, true},    // ENEMY_ANIM_WALK
	{6, 0.3f, true},    // ENEMY_ANIM_DASH
	{4, 0.3f, false},   // ENEMY_ANIM_DAMAGE
	{0, 0.2f, false},   // ENEMY_ANIM_ATTACK
	{1, 0.3f, false},   // ENEMY_ANIM_ATTACKCOOL
	{2, 0.25f, false},  // ENEMY_ANIM_DEATH
	{3, 0.3f, false},   // ENEMY_ANIM_DEATHPOSE

	// スケルトンノーマル独自のアニメーション
	
	{8, 0.2f, true},    // 勝利アニメーション
};


Skeleton_Normal::Skeleton_Normal(float startX, float startY, float startZ, int skeletonNModel, int TexHandle, SkeletonNormalStatus& noramalData, float growRate) {

	x = startX;
	y = startY;
	z = startZ;

	const EnemyStatus& data = noramalData.enemyStatus;

	// JSONファイルから読み込んだデータを代入
	// 基底クラスのDrawUI()でHPゲージ描画に使用
	maxHp = static_cast<int>(data.baseHp * growRate);

	hp = maxHp;

	attackPower = static_cast<int>(data.baseAttack * growRate);

	speed = data.speed;

	dashSpeed = data.dashSpeed;

	attackCoolTime = data.attackCoolTime;

	invincibleTime = data.invincibleTime;

	hideTime = data.hideTime;

	chaseRangeRadius = data.chaseRangeRadius;

	attackRangeRadius = data.attackRangeRadius;

	attackRadius = data.attackRadius;

	bodyRadius = data.bodyRadius;

	score = data.score;

	// 種類を設定
	enemyType = ENEMY_NORMAL;

	// 元モデルから複製
	modelHandle = MV1DuplicateModel(skeletonNModel);

	// テクスチャを適用
	MV1SetTextureGraphHandle(modelHandle, 0, TexHandle, FALSE);

	// 自分専用のテーブルをセット
	animTable = SKELETON_NORMAL_ANIM_TABLE;

	// 最初はスポーンアニメーションを再生
	SwitchAnimation(ENEMY_SPAWN);
	state = ENEMY_SPAWN;

	// モデルを拡大
	scale = data.scale;
	MV1SetScale(modelHandle, VGet(scale, scale, scale));

	// スポーン座標を設定
	MV1SetPosition(modelHandle, VGet(x, y, z));
}

void Skeleton_Normal::Update(Player& player, float deltaTime) {

	Enemy::Update(player, deltaTime);

	switch (state)
	{
		case ENEMY_SPAWN:
			
			UpdateSpawn();

			break;

		case ENEMY_PATROL:

			UpdatePatrol(player);

			break;

		case ENEMY_CHASE:

			UpdateChase(player);

			break;

		case ENEMY_ATTACK:

			UpdateAttack(player, deltaTime);

			break;

		case ENEMY_DAMAGE:

			UpdateDamage();

			break;

		case ENEMY_DEATH:

			UpdateDeath(deltaTime);

			break;

		case SKELETON_N_VICTORY:

			// 勝利アニメーションを再生
			SwitchAnimation(SKELETON_N_ANIM_VICTORY_POSE);

			break;

		default:

			break;
	}

	// プレイヤーが死亡した場合
	if (player.IsDead())
	{
		// 勝利アニメーション再生
		state = SKELETON_N_VICTORY;
	}
}

void Skeleton_Normal::UpdateSpawn() {

	SwitchAnimation(ENEMY_ANIM_SPAWN);

	// 総再生時間を超えた場合
	if (animTime >= totalTime)
	{
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}
}

void Skeleton_Normal::UpdateAttack(Player& player, float deltaTime) {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

	// 攻撃中
	if (currentAnim == ENEMY_ANIM_ATTACK)
	{
		// アニメーション再生時間の割合で攻撃判定を拡大縮小する
		float t = animTime / totalTime;

		// サイン波に変換
		float wave = sinf(t * DX_PI_F);

		// 最大で15伸びる
		float stepOffset = wave * 15.0f;

		// 構造体に代入
		hitBox.isAttackActive = true;
		hitBox.x = x - sinf(angleY) * stepOffset;
		hitBox.z = z - cosf(angleY) * stepOffset;
		hitBox.radius = attackRadius;

		// 攻撃終了したらクール開始
		if (animTime >= totalTime)
		{
			// 攻撃判定を無効
			hitBox.isAttackActive = false;

			SwitchAnimation(ENEMY_ANIM_ATTACKCOOL);
		}
	}
	else if (currentAnim == ENEMY_ANIM_ATTACKCOOL)
	{
		float dx = player.GetPosition().x - x;
		float dz = player.GetPosition().z - z;

		angleY = atan2f(dx, dz) + DX_PI_F;
		MV1SetRotationXYZ(modelHandle, VGet(0, angleY, 0));
		isAttackCoolTimer += deltaTime;

		if (isAttackCoolTimer >= attackCoolTime)
		{
			isAttackCoolTimer = 0.0f;

			// プレイヤーが攻撃範囲内にいる場合
			if (IsPlayerInAttackRange(player))
			{
				SwitchAnimation(ENEMY_ANIM_ATTACK);
				state = ENEMY_ATTACK;
				animTime = 0.0f;
			}
			else // 攻撃範囲外にいる場合
			{
				SwitchAnimation(ENEMY_ANIM_DASH);
				state = ENEMY_CHASE;
			}
		}
	}
}

void Skeleton_Normal::UpdateDamage() {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

	SwitchAnimation(ENEMY_ANIM_DAMAGE);
	if (animTime >= totalTime)
	{
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}
}

void Skeleton_Normal::UpdateDeath(float deltaTime) {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

	// 死亡アニメーション後に倒れるアニメーションに遷移
	if (animTime >= totalTime)
	{
		SwitchAnimation(ENEMY_ANIM_DEATHPOSE);
	}

	deathTimer += deltaTime;

	// 一定時間経過後に非アクティブ化
	if (deathTimer >= hideTime)
	{
		isActive = false;
	}
}

void Skeleton_Normal::DrawModel() const {

	MV1DrawModel(modelHandle);
}