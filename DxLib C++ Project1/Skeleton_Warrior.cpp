#include "Skeleton_Warrior.h"
#include "Player.h"
#include "EnemyWarriorDataLoader.h"
#include "GameConfig.h"
#include "DxLib.h"


#include "MagicEffectManager.h"

const EnemyAnimData SKELETON_WARRIOR_ANIM_TABLE[] = {

	// Skeleton_Warrior専用のアニメーションテーブル
	{8, 0.5f, false},   // ENEMY_ANIM_SPAWN
	{6, 0.3f, true},    // ENEMY_ANIM_IDLE
	{11, 0.3f, true},   // ENEMY_ANIM_WALK
	{7, 0.3f, true},    // ENEMY_ANIM_DASH
	{5, 0.3f, false},   // ENEMY_ANIM_DAMAGE
	{0, 0.4f, false},   // ENEMY_ANIM_ATTACK
	{1, 0.3f, false},   // ENEMY_ANIM_ATTACKCOOL
	{2, 0.25f, false},  // ENEMY_ANIM_DEATH
	{3, 0.3f, false},   // ENEMY_ANIM_DEATHPOSE

	// スケルトンウォーリアー独自のアニメーション
	{4, 0.3f, false},   // 復活アニメーション
};

Skeleton_Warrior::Skeleton_Warrior(float startX, float startY, float startZ, int skeletonWModel, int TexHandle, SkeletonWarriorStatus& warriorData, float growRate) {

	x = startX;
	y = startY;
	z = startZ;

	const EnemyStatus& data = warriorData.enemyStatus;

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

	// JSONファイルから読み込んだ固有データを代入
	reviveChancePercent = warriorData.reviveChancePercent;

	reviveWaitTime = warriorData.reviveWaitTime;

	reviveHpMultiplier = warriorData.reviveHpMultiplier;

	reviveScoreMultiplier = warriorData.reviveScoreMultiplier;


	// 種類を設定
	enemyType = ENEMY_WARRIOR;

	isRevived = false;

	willRevive = false;

	// 元モデルから複製
	modelHandle = MV1DuplicateModel(skeletonWModel);

	// テクスチャを適用
	MV1SetTextureGraphHandle(modelHandle, 0, TexHandle, FALSE);

	// 自分専用のテーブルをセット
	animTable = SKELETON_WARRIOR_ANIM_TABLE;

	// 最初はスポーンアニメーションを再生
	SwitchAnimation(ENEMY_SPAWN);
	state = ENEMY_SPAWN;

	// モデルを拡大
	scale = data.scale;
	MV1SetScale(modelHandle, VGet(scale, scale, scale));

	// スポーン座標を設定
	MV1SetPosition(modelHandle, VGet(x, y, z));
}

void Skeleton_Warrior::Update(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) {

	Enemy::Update(player, deltaTime, magicEffectManager);

	// 死亡中・復活中は無敵タイマーが切れないようにする
	if (state == ENEMY_DEATH || state == SKELETON_W_RESURRECTION)
	{
		isInvincible = true;
		invincibleTimer = 0.0f;
	}

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

			UpdateAttack(player, deltaTime, magicEffectManager);

			break;

		case ENEMY_DAMAGE:

			UpdateDamage();

			break;

		case ENEMY_DEATH:

			UpdateDeath(deltaTime);

			break;

		case SKELETON_W_RESURRECTION:

			UpdateRevive();

			break;

		case SKELETON_W_VICTORY:

			SwitchAnimation(ENEMY_ANIM_IDLE);

			break;

		default:

			break;
	}

	if (!player.IsActive())
	{
		state = SKELETON_W_VICTORY;
	}
}

void Skeleton_Warrior::UpdateSpawn() {

	SwitchAnimation(ENEMY_ANIM_SPAWN);

	// 総再生時間を超えた場合
	if (animTime >= totalTime)
	{
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}
}

void Skeleton_Warrior::UpdateAttack(Player& player, float deltaTime, MagicEffectManager& magicEffectManager) {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

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

void Skeleton_Warrior::UpdateDamage() {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

	SwitchAnimation(ENEMY_ANIM_DAMAGE);

	if (animTime >= totalTime)
	{
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}
}


bool Skeleton_Warrior::TakeDamage(int value, SoundManager& soundManager) {

	// 基底クラスの関数を呼びだし、死んだかどうかをチェック
	bool isDead = Enemy::TakeDamage(value, soundManager);

	// 死亡した場合に復活抽選
	if (isDead)
	{
		// 2度目の復活はしない
		if (isRevived) return true;

		std::uniform_int_distribution<int> reviveChance(REVIVE_CHANCE_MIN, REVIVE_CHANCE_MAX);

		int reviveCheck = reviveChance(rng);

		// JSONで設定した値以下だった場合復活
		if (reviveCheck <= reviveChancePercent)
		{
			// 復活可能にする
			willRevive = true;
			state = ENEMY_DEATH;
			SwitchAnimation(ENEMY_ANIM_DEATH);
			return false;
		}

		return true;
	}

	return false;
}

void Skeleton_Warrior::UpdateRevive() {

	isInvincible = true;

	if (animTime >= totalTime)
	{
		isInvincible = false;
		state = ENEMY_PATROL;
		SwitchAnimation(ENEMY_ANIM_WALK);
	}
}

void Skeleton_Warrior::UpdateDeath(float deltaTime) {

	// 攻撃判定を無効
	hitBox.isAttackActive = false;

	// 死亡アニメーション後に倒れるアニメーションに遷移
	if (animTime >= totalTime)
	{
		SwitchAnimation(ENEMY_ANIM_DEATHPOSE);
	}

	deathTimer += deltaTime;

	// 復活できる場合
	if (willRevive)
	{
		// 一定時間後に復活状態へ遷移
		if (deathTimer >= reviveWaitTime)
		{
			willRevive = false;

			// 復活は1度だけ
			isRevived = true;
			state = SKELETON_W_RESURRECTION;
			SwitchAnimation(SKELETON_W_REVIVE);

			// JSONで指定した倍率のHPで復活させる
			//hp = static_cast<int>(maxHp * reviveHpMultiplier);

			// JSONで指定した倍率のHPで復活させる
			hp = maxHp * 3 / 2;

			// HPゲージの最大値を設定 
			maxHp = hp;

			// 復活時はもう1度倒すことになるため、JSONで指定した倍率分スコアを加算
			score *= reviveScoreMultiplier;
			deathTimer = 0.0f;
		}
	}
	else // 一定時間経過後に非アクティブ化
	{
		if (deathTimer >= hideTime)
		{
			isActive = false;
		}
	}
}

void Skeleton_Warrior::DrawModel() const {

	MV1DrawModel(modelHandle);
}