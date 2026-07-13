#pragma once
#include "DxLib.h"
#include "PlayerStatus.h"
#include <vector>
#include <string>

// アニメーションの種類
enum PlayerAnimation {
	PLAYER_ANIM_SPAWN_AIR,      // スポーンアニメーション(パターン1)
	PLAYER_ANIM_SPAWN_GROUND,   // スポーンアニメーション(パターン2)
	PLAYER_ANIM_IDLE,           // 待機アニメーション
	PLAYER_ANIM_WALK,           // 歩きアニメーション
	PLAYER_ANIM_DASH,           // ダッシュアニメーション
	PLAYER_ANIM_DAMAGE,         // 被弾アニメーション
	PLAYER_ANIM_ATTACK,         // 攻撃アニメーション
	PLAYER_ANIM_DEATH,          // 死亡アニメーション(倒れる)
	PLAYER_ANIM_DEATHPOSE,      // 死亡アニメーション(待機)

	PLAYER_ANIM_MAX,
};

// 状態の種類
enum PlayerState {
	PLAYER_SPAWN,       // スポーン状態
	PLAYER_IDLE,        // 待機状態
	PLAYER_WALK,        // 歩き状態
	PLAYER_DASH,        // ダッシュ状態
	PLAYER_ATTACK,      // 攻撃状態
	PLAYER_DAMAGE,      // 被弾状態
	PLAYER_DEATH,       // 死亡状態
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


class Player {
public:

	Player();
	void Reset(); // ゲームリセット処理
	void Update(float deltaTime);

	// ----------------------------------------
	// ステータス変更
	// ----------------------------------------

	void TakeDamage(int value, SoundManager& soundManager);
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

	// 体の当たり判定半径
	float GetBodyRadius() const { return bodyRadius; }

private:

	// ----------------------------------------
	// 各状態の更新処理
	// ----------------------------------------

	void UpdateSpawn();
	void UpdateIdle();
	void UpdateWalk();
	void UpdateDash();
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

	void AnimSwitch(int animNum);

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

	// レベルアップテキストの表示時間(秒)
	float levelUpTextActiveTime = -1.0f;

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
	PlayerState state = PLAYER_SPAWN;

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
	// レベルアップ演出(カウンター)
	// ----------------------------------------

	// 画面に表示させるかどうか
	bool isLevelUpTextActive = false;
	float levelupTextActiveTimer = 0.0f;


	// ----------------------------------------
	// 攻撃判定
	// ----------------------------------------

	PlayerAttackHitBox attackHitBox;

	// ----------------------------------------
	// アニメーション
	// ----------------------------------------

	 // アタッチするアニメーションのハンドル
	int attachIndex;

	// 現在の再生時間
	float animTime;

	// アニメーション総再生時間
	float totalTime;

	// 現在のアニメーション番号を保持
	int currentAnim = -1;

	// 遷移先のアニメーション番号
	int targetAnim = -1;

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
};