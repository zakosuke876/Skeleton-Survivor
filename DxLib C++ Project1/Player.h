#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include "PlayerStatus.h"
#include "GameConfig.h"
#include "Sword.h"
#include "Effect.h"
#include "GroundEffect.h"

// アニメーションの種類
enum class PlayerAnimation {
	SpawnAir,      // スポーンアニメーション(パターン1)
	SpawnGround,   // スポーンアニメーション(パターン2)
	Idle,           // 待機アニメーション
	Walk,           // 歩きアニメーション
	Dash,           // ダッシュアニメーション
	Damage,         // 被弾アニメーション
	Attack,         // 攻撃アニメーション
	Death,          // 死亡アニメーション(倒れる)
	DeathPose,      // 死亡アニメーション(待機)
	Max,
};

// 状態の種類
enum class PlayerState {
	Spawn,       // スポーン状態
	Idle,        // 待機状態
	Walk,        // 歩き状態
	Dash,        // ダッシュ状態
	Attack,      // 攻撃状態
	Damage,      // 被弾状態
	Death,       // 死亡状態
};

struct PlayerAnimData {
	int animIndex;   // アニメーション番号
	float animSpeed; // 再生スピード
	bool loop;       // ループ設定
};

// 攻撃判定情報
struct PlayerAttackHitBox {
	float x, z;          // 座標
	float radius;        // 半径
	bool isAttackActive; // 当たり判定が有効か
};

// 前方宣言
class FontManager;
class SoundManager;
class Ground;
class Random;
class InputManager;
class SpriteManager;


class Player {
public:

	Player(Random& random, const InputManager& inputManager, const SpriteManager& spriteManager);
	void Reset(); // ゲームリセット処理
	void Update(float deltaTime, const Ground& ground);

	// ----------------------------------------
	// ステータス変更
	// ----------------------------------------

	void TakeDamage(int value, const SoundManager& soundManager);
	void Recovery(int value);
	void AddExp(int value, const SoundManager& soundManager);
	void LevelUp(const SoundManager& soundManager);

	// ----------------------------------------
	// 描画処理
	// ----------------------------------------

	void Draw() const;
	void DrawUI(const FontManager& fontManager) const;

	// ----------------------------------------
	// Getter
	// ----------------------------------------
	
	VECTOR GetPosition() const { return VGet(x, y, z); }
	bool IsDead() const { return isDead; }
	bool IsActive() const { return isActive; }
	int GetAttackPower() const { return attackPower; }

	// 当たり判定の計算に使う座標・半径・有効フラグを返す
	PlayerAttackHitBox GetPlayerAttackHitBox() const { return attackHitBox; }

	// エフェクトの生成位置を返す(体の中心)
	VECTOR GetBodyEffectPosition() const;

	// 体の中心に生成するエフェクトの生成位置 Y方向オフセット
	static constexpr float BODY_EFFECT_OFFSET_Y = 10.0f;

	// エフェクトの生成位置を返す(頭上)
	VECTOR GetHeadEffectPosition() const;

	// 頭上に生成するエフェクトの生成位置 Y方向オフセット
	static constexpr float HEAD_EFFECT_OFFSET_Y = 40.0f;

	// 無敵時間中かどうか返す
	bool IsInvincible() const { return isInvincible; }

	// 体の当たり判定半径
	float GetBodyRadius() const { return bodyRadius; }

	int GetHandFrame() const { return handFrameIndex; }

private:

	// Gameが所有する本体を参照で借用
	Random& random;
	const InputManager& inputManager;
	const SpriteManager& spriteManager;

	GroundEffect groundEffect;
	Effect levelUpEffect;
	Effect deathEffect;
	Effect recoveryEffect;
	Effect expEffect;

	// ----------------------------------------
	// 各状態の更新処理
	// ----------------------------------------

	void UpdateSpawn();
	void UpdateIdle();
	void UpdateWalk(const Ground& ground);
	void UpdateDash(const Ground& ground);
	void UpdateAttack();
	void UpdateDamage();
	void UpdateDeath(float deltaTime);

	// ----------------------------------------
	// 移動・位置制限
	// ----------------------------------------

	void Move(float moveSpeed);

	// 移動範囲を制限
	void ClampPosition();

	// ----------------------------------------
	// 入力判定
	// ----------------------------------------

	bool IsMoveInput();
	bool IsDashInput();
	bool IsAttackInput();

	// ----------------------------------------
	// 描画処理
	// ----------------------------------------

	void DrawHpGauge(const FontManager& fontManager)  const;
	void DrawExpGauge(const FontManager& fontManager) const;

	// ----------------------------------------
	// 死亡処理
	// ----------------------------------------

	void Die();

	// ----------------------------------------
	// アニメーション設定
	// ----------------------------------------

	void AnimSwitch(PlayerAnimation animNum);

	// JSONから読み込む値(PlayerStatusに対応)
	// ----------------------------------------
	// ステータス
	// ----------------------------------------

	// 現在HP
	int hp = -1;

	// 最大HP(HPゲージ描画に使う)
	int maxHp = -1;

	int attackPower = -1;
	float speed = -1.0f;
	float dashMultiplier = -1.0f;
	int level = -1;
	int nextLevelExp = -1;

	// レベルアップで上がる攻撃力
	int levelUpAttackPower = -1;

	// レベルアップに必要な経験値の倍率
	float nextLevelExpMultiplier = -1.0f;

	// ----------------------------------------
	// 当たり判定
	// ----------------------------------------

	// 体の当たり判定半径
	float bodyRadius = -1.0f;

	// 攻撃判定の半径
	float attackRadius = -1.0f;

	// ----------------------------------------
	// 時間パラメータ
	// ----------------------------------------

	// 無敵時間(秒)
	float invincibleTime = -1.0f;

	// 死亡後消えるまでの時間(秒)
	float hideTime = -1.0f;

	// ----------------------------------------
	// モデル・描画
	// ----------------------------------------

	// プレイヤーの3Dモデルハンドル
	int modelHandle;

	// プレイヤーのテクスチャハンドル
	int playerTexHandle;

	// モデルの拡大倍率
	float scale = -1.0f;

	// ----------------------------------------
	// 座標・向き
	// ----------------------------------------
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float angleY = 0.0f;

	// ----------------------------------------
	// 経験値
	// ----------------------------------------

	int exp = -1;
	
	// ----------------------------------------
	// フラグ・状態
	// ----------------------------------------

	// オブジェクトが有効かどうか
	bool isActive = true;

	// 死亡しているか
	bool isDead = false;

	// 攻撃中かどうか
	bool isAttack = false;

	// 現在の状態
	PlayerState state = PlayerState::Spawn;

	// ----------------------------------------
	// 無敵時間(カウンター)
	// ----------------------------------------

	// 現在無敵状態かどうか
	bool isInvincible = false;
	float invincibleTimer = 0.0f;

	// ----------------------------------------
	// 死亡処理(カウンター)
	// ----------------------------------------

	float deathTimer = 0.0f;

	// ----------------------------------------
	// 攻撃判定
	// ----------------------------------------

	PlayerAttackHitBox attackHitBox;

	// ----------------------------------------
	// アニメーション
	// ----------------------------------------

	 // アタッチするアニメーションのハンドル
	int attachIndex = -1;

	// 現在の再生時間
	float animTime = 0.0f;

	// アニメーション総再生時間
	float totalTime = 0.0f;

	// 現在のアニメーション番号を保持
	int currentAnim = -1;

	// 遷移先のアニメーション番号
	int targetAnim = -1;

	// 攻撃判定の有効フレーム
	static constexpr float ATTACK_HIT_START_FRAME = 10.0f;
	static constexpr float ATTACK_HIT_END_FRAME   = 18.0f;

	// 死亡演出の上方向オフセット
	static constexpr float DEATH_EFFECT_Y_OFFSET = 40.0f;

	// ----------------------------------------
	// UI描画用定数
	// ----------------------------------------

	// HPゲージ表示の座標
	static constexpr int PLAYER_HPGAUGE_X = 100;
	static constexpr int PLAYER_HPGAUGE_Y = 100;
	static constexpr int PLAYER_HPGAUGE_BOTTOM = 20;

	// HPゲージ全体の横幅
	static constexpr int HP_GAUGE_WIDTH = 200;

	// HP半分以下でゲージを黄色にする
	static constexpr float HP_GAUGE_YELLOW_VALUE = 0.5f;

	// EXPゲージのHPゲージからの縦オフセット
	static constexpr int EXP_GAUGE_OFFSET_Y = 50;

	// レベルテキストの縦Yオフセット
	static constexpr int LV_TEXT_OFFSET_Y = 2;

	// レベルテキストの座標
	static constexpr int LV_TEXT_X = 10;

	// ----------------------------------------
	// JSONデータ保持
	// ----------------------------------------

	// JSONから読み込んだプレイヤーのステータス設定値を保持する
	PlayerStatus playerDataTable;

	// ----------------------------------------
	// 武器のモデルハンドル
	// ----------------------------------------

	int swordHandle = -1;
	int swordTexHandle = -1;
	int handFrameIndex = -1;

	static constexpr float SWORD_SCALE = 0.01f;

	// 剣
	Sword sword;
};
