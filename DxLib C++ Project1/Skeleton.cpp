/*#include "Skeleton.h"
#include "Player.h"
#include "GameConfig.h"
#include "DxLib.h"
#include <cmath>

const SkeletonAnimData SKELETON_ANIM_TABLE[] = {
	// アニメーション番号, アニメーション再生スピード, ループ設定
	{7, 0.3f, false}, // 0
	{5, 0.3f, false},
	{9, 0.3f, true},
	{6, 0.3f, true},
	{4, 0.3f, false},
	{0, 0.2f, false},
	{1, 0.3f, false},
	{2, 0.25f, false},
	{3, 0.3f, false},
	{8, 0.2f, true},  // 9
};

Skeleton::Skeleton(float startX, float startY, float startZ) {

	// ランダム生成用
	std::random_device rd;
	rng = std::mt19937(rd());

	x = startX;
	y = startY;
	z = startZ;
	patrolBaseX = x;
	patrolBaseZ = z;
	isActive = true;

	// モデルの読み込み
	skeletonHandle = MV1LoadModel("Model/Enemy/Skeleton_Kanse2.mv1");

	// テクスチャ反映
	skeletonTexHandle = LoadGraph("Model/Enemy/skeleton_texture.png");
	MV1SetTextureGraphHandle(skeletonHandle, 0, skeletonTexHandle, FALSE);

	MV1SetScale(skeletonHandle, VGet(SKELETON_SCALE, SKELETON_SCALE, SKELETON_SCALE));

	SpawnAnimation(SKELETON_ANIM_SPAWN);
	state = SKELETON_SPAWN;

	// モデルを原点に配置
	MV1SetPosition(skeletonHandle, VGet(x, y, z));
}

void Skeleton::SpawnAnimation(int animationNum) {

	MV1DetachAnim(skeletonHandle, attachIndex);

	attachIndex = MV1AttachAnim(skeletonHandle, SKELETON_ANIM_TABLE[animationNum].animIndex);

	totalTime = MV1GetAttachAnimTotalTime(skeletonHandle, attachIndex);
	animTime = 0.0f;

	currentAnim = animationNum;
}

void Skeleton::AnimationSet(int animationNum)
{
	// // 違うアニメーションなら切り替える
	if (currentAnim != animationNum)
	{
		// アタッチされているアニメーションを削除
		MV1DetachAnim(skeletonHandle, attachIndex);

		// 新しいアニメーションをアタッチ
		attachIndex = MV1AttachAnim(skeletonHandle, SKELETON_ANIM_TABLE[animationNum].animIndex);

		// 総再生時間を取得
		totalTime = MV1GetAttachAnimTotalTime(skeletonHandle, attachIndex);

		// 再生時間をリセット
		animTime = 0.0f;

		// 現在再生中のアニメーション番号を保持
		currentAnim = animationNum;
	}
}

void Skeleton::TakeDamage(int value) {

	if (isInvincible) return;

	AnimationSet(SKELETON_ANIM_DAMAGE);

	// 無敵時間開始
	isInvincible = true;

	state = SKELETON_DAMAGE;

	hp -= value;

	if (hp <= 0)
	{
		hp = 0;
		AnimationSet(SKELETON_ANIM_DEATH);
		state = SKELETON_DEATH;
	}
}

void Skeleton::Update(Player& player, float deltaTime) {

	switch (state)
	{
	case SKELETON_SPAWN:

		UpdateSpawn(player);
		//printfDx("UpdateSpawn\n");
		break;

	case SKELETON_PATROL:

		UpdatePatrol(player);
		//printfDx("UpdatePatrol\n");
		break;

	case SKELETON_CHASE:

		UpdateChase(player);
		//printfDx("UpdateChase\n");
		break;

	case SKELETON_ATTACK:

		UpdateAttack(player, deltaTime);
		//printfDx("UpdateAttack\n");
		break;

	case SKELETON_DAMAGE:

		UpdateDamage(player);
		//printfDx("UpdateDamage\n");

		break;

	case SKELETON_DEATH:

		UpdateDeath(deltaTime);
		//printfDx("UpdateDeath\n");

		break;

	case SKELETON_VICTORY:

		UpdateVictory();
		printfDx("UpdateVictory\n");
	}

	if (isInvincible)
	{
		invincibleTimer += deltaTime;

		// 無敵時間終了時にタイマーをリセット
		if (invincibleTimer >= INVINCIBLE_TIME)
		{
			invincibleTimer = 0.0f;
			isInvincible = false;
		}
	}

	// アニメーションタイマーを進める
	animTime += SKELETON_ANIM_TABLE[currentAnim].animSpeed;

	// 総再生時間を超えたらループ
	if (animTime >= totalTime)
	{
		// アニメーションをループさせるか
		if (SKELETON_ANIM_TABLE[currentAnim].loop)
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

	// プレイヤー死亡時に勝利状態へ遷移
	if (player.IsDead())
	{
		AnimationSet(SKELETON_ANIM_VICTORY);
		state = SKELETON_VICTORY;
	}

	// 再生時間を反映
	MV1SetAttachAnimTime(skeletonHandle, attachIndex, animTime);
}

void Skeleton::UpdateSpawn(const Player& player) {

	// スポーンアニメーションの再生が終了した場合
	if (currentAnim == SKELETON_ANIM_SPAWN && animTime >= totalTime)
	{
		state = SKELETON_PATROL;
		AnimationSet(SKELETON_ANIM_WALK);
	}
}

void Skeleton::UpdatePatrol(const Player& player) {

	if (!hasTargetPosition)
	{
		// 目的地X,Z座標の範囲設定
		std::uniform_real_distribution<float> distanceX(GameConfig::FIELD_MIN_X, GameConfig::FIELD_MAX_X);
		std::uniform_real_distribution<float> distanceZ(GameConfig::FIELD_MIN_Z, GameConfig::FIELD_MAX_Z);

		// ランダムな座標を目的地に設定
		targetX = distanceX(rng);
		targetZ = distanceZ(rng);

		// 目的地到着まで再計算しない
		hasTargetPosition = true;
	}

	// 差分を求める
	float dx = targetX - x;
	float dz = targetZ - z;

	// 距離を求める
	float distance = sqrtf(dx * dx + dz * dz);

	// 正規化
	float dirX = dx / distance;
	float dirZ = dz / distance;

	// 何度の方向にいるか計算, プレイヤーの方向に向ける(180度補正でモデルの向きを調整)
	angleY = atan2f(dx, dz) + DX_PI_F;
	MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));

	// 移動
	x += dirX * speed;
	z += dirZ * speed;

	// 目的地付近に到着した場合
	if (distance <= 5.0f)
	{
		// 目的地を再計算
		hasTargetPosition = false;
	}

	MV1SetPosition(skeletonHandle, VGet(x, y, z));

	// プレイヤーが検知範囲内に入ったか
	if (IsPlayerInRange(player))
	{
		state = SKELETON_CHASE;
		AnimationSet(SKELETON_ANIM_DASH);
	}
}

void Skeleton::UpdateChase(const Player& player) {

	// プレイヤーが検知範囲外に出たか
	if (!IsPlayerInRange(player))
	{
		state = SKELETON_PATROL;
		AnimationSet(SKELETON_ANIM_WALK);
	}

	// プレイヤーが攻撃範囲内に入ったか
	if (IsPlayerInAttackRange(player))
	{
		state = SKELETON_ATTACK;
		AnimationSet(SKELETON_ANIM_ATTACK);
	}

	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	float distance = sqrtf(dx * dx + dz * dz);

	float dirX = dx / distance;
	float dirZ = dz / distance;

	angleY = atan2f(dx, dz) + DX_PI_F;
	MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));

	DrawFormatString(100, 300, GetColor(255, 0, 0), "移動X : %f", dirX);
	DrawFormatString(100, 400, GetColor(255, 0, 0), "移動Z : %f", dirZ);
	DrawFormatString(100, 500, GetColor(255, 0, 0), "目的地 : %f : %f", dx, dz);

	x += dirX * dashSpeed;
	z += dirZ * dashSpeed;

	MV1SetPosition(skeletonHandle, VGet(x, y, z));
}

void Skeleton::UpdateAttack(Player& player, float deltaTime) {

	// 攻撃中
	if (currentAnim == SKELETON_ANIM_ATTACK)
	{
		// アニメーション再生時間の割合で攻撃判定を拡大縮小する
		float t = animTime / totalTime;

		// サイン波に変換
		float wave = sinf(t * DX_PI_F);

		// 最大で15伸びる
		float stepOffset = wave * 15.0f;

		skeletonAttackHitBox.isAttackActive = true;
		skeletonAttackHitBox.x = x - sinf(angleY) * stepOffset;
		skeletonAttackHitBox.z = z - cosf(angleY) * stepOffset;
		skeletonAttackHitBox.radius = SKELETON_ATTACK_RADIUS;

		// 攻撃終了したらクール開始
		if (animTime >= totalTime)
		{
			AnimationSet(SKELETON_ANIM_ATTACKCOOL);
		}
	}
	// クールタイム中
	else if (currentAnim == SKELETON_ANIM_ATTACKCOOL)
	{
		// 攻撃判定を無効
		skeletonAttackHitBox.isAttackActive = false;

		float dx = player.GetPosition().x - x;
		float dz = player.GetPosition().z - z;

		angleY = atan2f(dx, dz) + DX_PI_F;
		MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));
		isAttackCoolTimer += deltaTime;

		if (isAttackCoolTimer >= ATTACK_COOL_TIME)
		{
			isAttackCoolTimer = 0.0f;

			// プレイヤーが攻撃範囲内にいる場合
			if (IsPlayerInAttackRange(player))
			{
				AnimationSet(SKELETON_ANIM_ATTACK);
				state = SKELETON_ATTACK;
				animTime = 0.0f;
			}
			else // 攻撃範囲外にいる場合
			{
				state = SKELETON_CHASE;
				AnimationSet(SKELETON_ANIM_DASH);
			}
		}
	}
}

void Skeleton::UpdateDamage(const Player& player) {

	AnimationSet(SKELETON_ANIM_DAMAGE);

	if (animTime >= totalTime)
	{
		state = SKELETON_PATROL;
	}
}

void Skeleton::UpdateDeath(float deltaTime) {

	// 死亡アニメーション後に倒れるアニメーションに遷移
	if (animTime >= totalTime)
	{
		AnimationSet(SKELETON_ANIM_DEATHPOSE);
	}

	deathTimer += deltaTime;

	// 一定時間経過後に非アクティブ化
	if (deathTimer >= HIDE_TIME)
	{
		isActive = false;
	}
}

void Skeleton::UpdateVictory() {

}

bool Skeleton::IsPlayerInRange(const Player& player) const {

	// 自分とプレイヤーの距離が±120未満の場合CHASE状態へ
	float dx = x - player.GetPosition().x;
	float dz = z - player.GetPosition().z;

	return (dx * dx + dz * dz) < (chaseRangeRadius * chaseRangeRadius);
}

bool Skeleton::IsPlayerInAttackRange(const Player& player) const {

	// 自分とプレイヤーの距離が±30未満の場合
	float dx = x - player.GetPosition().x;
	float dz = z - player.GetPosition().z;

	return (dx * dx + dz * dz) < (attackRangeRadius * attackRangeRadius);
}

void Skeleton::Draw() const {

	if (!isActive) return;

	// モデルを描画
	MV1DrawModel(skeletonHandle);

	// デバッグ用
	DrawFormatString(250, 650, GetColor(255, 0, 0), "Skeleton:X座標%f", x);
	DrawFormatString(250, 700, GetColor(255, 0, 0), "Skeleton:Z座標%f", z);

	DrawFormatString(0, 60, GetColor(255, 0, 0), "Enemy: HP%d", hp);
}*/









/*#include "Skeleton.h"
#include "Player.h"
#include "GameConfig.h"
#include "DxLib.h"
#include <cmath>

const SkeletonAnimData SKELETON_ANIM_TABLE[] = {
	// アニメーション番号, アニメーション再生スピード, ループ設定
	{7, 0.3f, false}, // 0
	{5, 0.3f, false},
	{9, 0.3f, true},
	{6, 0.3f, true},
	{4, 0.3f, false},
	{0, 0.2f, false},
	{1, 0.3f, false},
	{2, 0.25f, false},
	{3, 0.3f, false},
	{8, 0.2f, true},  // 9
};

Skeleton::Skeleton(float startX, float startY, float startZ) {

	// ランダム生成用
	std::random_device rd;
	rng = std::mt19937(rd());

	x = startX;
	y = startY;
	z = startZ;
	patrolBaseX = x;
	patrolBaseZ = z;
	isActive = true;

	// モデルの読み込み
	skeletonHandle = MV1LoadModel("Model/Enemy/Skeleton_Kanse2.mv1");

	// テクスチャ反映
	skeletonTexHandle = LoadGraph("Model/Enemy/skeleton_texture.png");
	MV1SetTextureGraphHandle(skeletonHandle, 0, skeletonTexHandle, FALSE);

	MV1SetScale(skeletonHandle, VGet(SKELETON_SCALE, SKELETON_SCALE, SKELETON_SCALE));

	SpawnAnimation(SKELETON_ANIM_SPAWN);
	state = SKELETON_SPAWN;

	// モデルを原点に配置
	MV1SetPosition(skeletonHandle, VGet(x, y, z));
}

void Skeleton::SpawnAnimation(int animationNum) {

	MV1DetachAnim(skeletonHandle, attachIndex);

	attachIndex = MV1AttachAnim(skeletonHandle, SKELETON_ANIM_TABLE[animationNum].animIndex);

	totalTime = MV1GetAttachAnimTotalTime(skeletonHandle, attachIndex);
	animTime = 0.0f;

	currentAnim = animationNum;
}

void Skeleton::AnimationSet(int animationNum)
{
	// // 違うアニメーションなら切り替える
	if (currentAnim != animationNum)
	{
		// アタッチされているアニメーションを削除
		MV1DetachAnim(skeletonHandle, attachIndex);

		// 新しいアニメーションをアタッチ
		attachIndex = MV1AttachAnim(skeletonHandle, SKELETON_ANIM_TABLE[animationNum].animIndex);

		// 総再生時間を取得
		totalTime = MV1GetAttachAnimTotalTime(skeletonHandle, attachIndex);

		// 再生時間をリセット
		animTime = 0.0f;

		// 現在再生中のアニメーション番号を保持
		currentAnim = animationNum;
	}
}

void Skeleton::TakeDamage(int value) {

	if (isInvincible) return;

	AnimationSet(SKELETON_ANIM_DAMAGE);

	// 無敵時間開始
	isInvincible = true;

	state = SKELETON_DAMAGE;

	hp -= value;

	if (hp <= 0)
	{
		hp = 0;
		AnimationSet(SKELETON_ANIM_DEATH);
		state = SKELETON_DEATH;
	}
}

void Skeleton::Update(Player& player, float deltaTime) {

	switch (state)
	{
	case SKELETON_SPAWN:

		UpdateSpawn(player);
		//printfDx("UpdateSpawn\n");
		break;

	case SKELETON_PATROL:

		UpdatePatrol(player);
		//printfDx("UpdatePatrol\n");
		break;

	case SKELETON_CHASE:

		UpdateChase(player);
		//printfDx("UpdateChase\n");
		break;

	case SKELETON_ATTACK:

		UpdateAttack(player, deltaTime);
		//printfDx("UpdateAttack\n");
		break;

	case SKELETON_DAMAGE:

		UpdateDamage(player);
		//printfDx("UpdateDamage\n");

		break;

	case SKELETON_DEATH:

		UpdateDeath(deltaTime);
		//printfDx("UpdateDeath\n");

		break;

	case SKELETON_VICTORY:

		UpdateVictory();
		printfDx("UpdateVictory\n");
	}

	if (isInvincible)
	{
		invincibleTimer += deltaTime;

		// 無敵時間終了時にタイマーをリセット
		if (invincibleTimer >= INVINCIBLE_TIME)
		{
			invincibleTimer = 0.0f;
			isInvincible = false;
		}
	}

	// アニメーションタイマーを進める
	animTime += SKELETON_ANIM_TABLE[currentAnim].animSpeed;

	// 総再生時間を超えたらループ
	if (animTime >= totalTime)
	{
		// アニメーションをループさせるか
		if (SKELETON_ANIM_TABLE[currentAnim].loop)
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

	// プレイヤー死亡時に勝利状態へ遷移
	if (player.IsDead())
	{
		AnimationSet(SKELETON_ANIM_VICTORY);
		state = SKELETON_VICTORY;
	}

	// 再生時間を反映
	MV1SetAttachAnimTime(skeletonHandle, attachIndex, animTime);
}

void Skeleton::UpdateSpawn(const Player& player) {

	// スポーンアニメーションの再生が終了した場合
	if (currentAnim == SKELETON_ANIM_SPAWN && animTime >= totalTime)
	{
		state = SKELETON_PATROL;
		AnimationSet(SKELETON_ANIM_WALK);
	}
}

void Skeleton::UpdatePatrol(const Player& player) {

	if (!hasTargetPosition)
	{
		// 目的地X,Z座標の範囲設定
		std::uniform_real_distribution<float> distanceX(GameConfig::FIELD_MIN_X, GameConfig::FIELD_MAX_X);
		std::uniform_real_distribution<float> distanceZ(GameConfig::FIELD_MIN_Z, GameConfig::FIELD_MAX_Z);

		// ランダムな座標を目的地に設定
		targetX = distanceX(rng);
		targetZ = distanceZ(rng);

		// 目的地到着まで再計算しない
		hasTargetPosition = true;
	}

	// 差分を求める
	float dx = targetX - x;
	float dz = targetZ - z;

	// 距離を求める
	float distance = sqrtf(dx * dx + dz * dz);

	// 正規化
	float dirX = dx / distance;
	float dirZ = dz / distance;

	// 何度の方向にいるか計算, プレイヤーの方向に向ける(180度補正でモデルの向きを調整)
	angleY = atan2f(dx, dz) + DX_PI_F;
	MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));

	// 移動
	x += dirX * speed;
	z += dirZ * speed;

	// 目的地付近に到着した場合
	if (distance <= 5.0f)
	{
		// 目的地を再計算
		hasTargetPosition = false;
	}

	MV1SetPosition(skeletonHandle, VGet(x, y, z));
	
	// プレイヤーが検知範囲内に入ったか
	if (IsPlayerInRange(player))
	{
		state = SKELETON_CHASE;
		AnimationSet(SKELETON_ANIM_DASH);
	}
}

void Skeleton::UpdateChase(const Player& player) {

	// プレイヤーが検知範囲外に出たか
	if (!IsPlayerInRange(player))
	{
		state = SKELETON_PATROL;
		AnimationSet(SKELETON_ANIM_WALK);
	}

	// プレイヤーが攻撃範囲内に入ったか
	if (IsPlayerInAttackRange(player))
	{
		state = SKELETON_ATTACK;
		AnimationSet(SKELETON_ANIM_ATTACK);
	}

	float dx = player.GetPosition().x - x;
	float dz = player.GetPosition().z - z;

	float distance = sqrtf(dx * dx + dz * dz);

	float dirX = dx / distance;
	float dirZ = dz / distance;

	angleY = atan2f(dx, dz) + DX_PI_F;
	MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));

	DrawFormatString(100, 300, GetColor(255, 0, 0), "移動X : %f", dirX);
	DrawFormatString(100, 400, GetColor(255, 0, 0), "移動Z : %f", dirZ);
	DrawFormatString(100, 500, GetColor(255, 0, 0), "目的地 : %f : %f", dx, dz);

	x += dirX * dashSpeed;
	z += dirZ * dashSpeed;

	MV1SetPosition(skeletonHandle, VGet(x, y, z));
}

void Skeleton::UpdateAttack(Player& player, float deltaTime) {

	// 攻撃中
	if (currentAnim == SKELETON_ANIM_ATTACK)
	{
		// アニメーション再生時間の割合で攻撃判定を拡大縮小する
		float t = animTime / totalTime;

		// サイン波に変換
		float wave = sinf(t * DX_PI_F);

		// 最大で15伸びる
		float stepOffset = wave * 15.0f;

		skeletonAttackHitBox.isAttackActive = true;
		skeletonAttackHitBox.x = x - sinf(angleY) * stepOffset;
		skeletonAttackHitBox.z = z - cosf(angleY) * stepOffset;
		skeletonAttackHitBox.radius = SKELETON_ATTACK_RADIUS;

		// 攻撃終了したらクール開始
		if (animTime >= totalTime)
		{
			AnimationSet(SKELETON_ANIM_ATTACKCOOL);
		}
	}
	// クールタイム中
	else if (currentAnim == SKELETON_ANIM_ATTACKCOOL)
	{
		// 攻撃判定を無効
		skeletonAttackHitBox.isAttackActive = false;

		float dx = player.GetPosition().x - x;
		float dz = player.GetPosition().z - z;

		angleY = atan2f(dx, dz) + DX_PI_F;
		MV1SetRotationXYZ(skeletonHandle, VGet(0, angleY, 0));
		isAttackCoolTimer += deltaTime;

		if (isAttackCoolTimer >= ATTACK_COOL_TIME)
		{
			isAttackCoolTimer = 0.0f;

			// プレイヤーが攻撃範囲内にいる場合
			if (IsPlayerInAttackRange(player))
			{
				AnimationSet(SKELETON_ANIM_ATTACK);
				state = SKELETON_ATTACK;
				animTime = 0.0f;
			}
			else // 攻撃範囲外にいる場合
			{
				state = SKELETON_CHASE;
				AnimationSet(SKELETON_ANIM_DASH);
			}
		}
	}
}

void Skeleton::UpdateDamage(const Player& player){

	AnimationSet(SKELETON_ANIM_DAMAGE);

	if (animTime >= totalTime)
	{
		state = SKELETON_PATROL;
	}
}

void Skeleton::UpdateDeath(float deltaTime) {

	// 死亡アニメーション後に倒れるアニメーションに遷移
	if (animTime >= totalTime)
	{
		AnimationSet(SKELETON_ANIM_DEATHPOSE);
	}

	deathTimer += deltaTime;

	// 一定時間経過後に非アクティブ化
	if (deathTimer >= HIDE_TIME)
	{
		isActive = false;
	}
}

void Skeleton::UpdateVictory() {

}

bool Skeleton::IsPlayerInRange(const Player& player) const {

	// 自分とプレイヤーの距離が±120未満の場合CHASE状態へ
	float dx = x - player.GetPosition().x;
	float dz = z - player.GetPosition().z;

	return (dx * dx + dz * dz) < (chaseRangeRadius * chaseRangeRadius);
}

bool Skeleton::IsPlayerInAttackRange(const Player& player) const {

	// 自分とプレイヤーの距離が±30未満の場合
	float dx = x - player.GetPosition().x;
	float dz = z - player.GetPosition().z;

	return (dx * dx + dz * dz) < (attackRangeRadius * attackRangeRadius);
}

void Skeleton::Draw() const {

	if (!isActive) return;

	// モデルを描画
	MV1DrawModel(skeletonHandle);

	// デバッグ用
	DrawFormatString(250, 650, GetColor(255, 0, 0), "Skeleton:X座標%f", x);
	DrawFormatString(250, 700, GetColor(255, 0, 0), "Skeleton:Z座標%f", z);

	DrawFormatString(0, 60, GetColor(255, 0, 0), "Enemy: HP%d", hp);
}*/