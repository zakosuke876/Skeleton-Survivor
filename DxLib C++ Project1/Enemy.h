#pragma once
#include "EnemyType.h"
#include "DxLib.h"
#include <random>
#include <string>

// 前方宣言
class Player;
class SoundManager;

// 共通アニメーションの番号
// 各敵のアニメーションはこの順番に合わせて定義する
enum EnemyAnimation {
	ENEMY_ANIM_SPAWN,      // スポーンアニメーション
	ENEMY_ANIM_IDLE,       // 待機アニメーション
	ENEMY_ANIM_WALK,       // 歩きアニメーション
	ENEMY_ANIM_DASH,       // ダッシュアニメーション
	ENEMY_ANIM_DAMAGE,     // 被弾アニメーション
	ENEMY_ANIM_ATTACK,     // 攻撃アニメーション
	ENEMY_ANIM_ATTACKCOOL, // 攻撃待機アニメーション
	ENEMY_ANIM_DEATH,      // 死亡アニメーション(吹き飛び)
	ENEMY_ANIM_DEATHPOSE,  // 死亡アニメーション(倒れる)

	ENEMY_ANIM_MAX,
};

// 共通ステート
// 子クラス独自のステートはENEMY_STATE_MAXから続きの番号で定義する
enum EnemyState {
	ENEMY_SPAWN,         // スポーン状態
	ENEMY_PATROL,		 // プレイヤー索敵状態
	ENEMY_CHASE,         // プレイヤー追跡状態
	ENEMY_ATTACK,        // 攻撃状態
	ENEMY_DAMAGE,        // 被弾状態
	ENEMY_DEATH,         // 死亡状態

	ENEMY_STATE_MAX,
};

// アニメーションのデータ
struct EnemyAnimData {
	int animIndex;   // アニメーション番号
	float animSpeed; // 再生スピード
	bool loop;       // ループ再生するか
};


// 敵の当たり判定をまとめた構造体
struct EnemyAttackHitBox {
	float x, z;           // 座標
	float radius;         // 半径
	bool isAttackActive;  // 当たり判定が有効か
};

// JSONから読み込んだ敵の共通ステータスを格納する構造体
// 読み込んだ値をEnemy派生クラスのメンバ変数へコピーして使用
struct EnemyStatus {
	int baseHp;              // 基礎HP(growRateで倍率をかける前の値)
	int baseAttack;          // 基礎攻撃力(growRateで倍率をかける前の値)
	float speed;             // 歩くスピード
	float dashSpeed;         // 走るスピード
	float attackCoolTime;    // 攻撃クールタイム(秒)
	float invincibleTime;    // 無敵時間(秒)
	float hideTime;          // 死亡後に消えるまでの時間(秒)
	float chaseRangeRadius;  // チェイス状態へ遷移する距離
	float attackRangeRadius; // 攻撃状態へ遷移する距離
	float attackRadius;      // 攻撃判定の半径
	float bodyRadius;        // 体の当たり判定半径
	int score;               // 倒した時に加算されるスコア
	float scale;             // モデルの拡大倍率
	std::string modelPath;   // モデルファイルのパス
	std::string texturePath; // テクスチャファイルのパス
};

// 敵の基底クラス
// アニメーション切り替え・リソース管理を実装
class Enemy {
public:

	Enemy();
	virtual ~Enemy();

	virtual void Update(Player& player, float deltaTime);
	void Draw() const;
	void DrawUI() const;

	// ----------------------------------------
	// Getter
	// ----------------------------------------

	VECTOR GetPos() const { return VGet(x, y, z); }

	// 当たり判定の計算に使う座標・半径・有効フラグを返す
	EnemyAttackHitBox GetHitBox() const { return hitBox; }

	// 体の当たり判定半径
	float GetBodyRadius() const { return bodyRadius; }

	bool IsActive() const { return isActive; }
	int GetAttackPower() const { return attackPower; }
	int GetScoreValue()  const { return score; }
	int GetHp() const { return hp; }

	virtual bool TakeDamage(int value, SoundManager& soundManager);
	virtual EnemyType GetEnemyType() const = 0;

protected:

	// ----------------------------------------
	// アニメーション・状態管理
	// ----------------------------------------

	void SwitchAnimation(int animationNum);
	void UpdateAnim();
	void Die();

	// ----------------------------------------
	// プレイヤー距離判定
	// ----------------------------------------

	bool IsPlayerInChaseRange(const Player& player) const;
	bool IsPlayerInAttackRange(const Player& player) const;

	// ----------------------------------------
	// 各状態の更新処理(純粋仮想関数)
	// ----------------------------------------

	virtual void UpdateSpawn() = 0;
	virtual void UpdatePatrol(const Player& player) = 0;
	virtual void UpdateChase(const Player& player) = 0;
	virtual void UpdateAttack(Player& player, float DeltaTime) = 0;
	virtual void UpdateDamage() = 0;
	virtual void UpdateDeath(float deltaTime) = 0;
	virtual void DrawModel() const = 0;

	// ----------------------------------------
	// 座標・向き
	// ----------------------------------------

	float x = 0.0f; // 座標
	float y = 0.0f;
	float z = 0.0f;

	// Y軸回転角度(敵をプレイヤーに向ける為に使う)
	float angleY = 0.0f;

	EnemyType enemyType;

	// 子クラス独自ステートにも遷移できるようにint型で定義
	int state = ENEMY_SPAWN;

	// ----------------------------------------
	// ステータス
	// ----------------------------------------

	int hp = -1;
	int maxHp = -1;
	int attackPower = -1;
	int totalScore = -1;
	int score = -1;

	// 移動速度
	float speed = -1.0f;
	float dashSpeed = -1.0f;

	// プレイヤーとの距離がこの値以下になった場合に該当する状態へ遷移
	float chaseRangeRadius = -1;
	float attackRangeRadius = -1;

	// ----------------------------------------
	// ステータス定数
	// ----------------------------------------

	// 攻撃クールタイム(秒)
	//static constexpr float ATTACK_COOL_TIME = 1.0f;

	// 攻撃クールタイム(秒)
	float attackCoolTime = -1.0f;

	// 無敵時間(秒)
	float invincibleTime = -1.0f;

	// 死亡後に消えるまでの時間(秒)
	float hideTime = -1.0f;

	// 無敵時間(秒)
	//static constexpr float INVINCIBLE_TIME = 2.0f;

	// 死亡後に消えるまでの時間(秒)
	//static constexpr float HIDE_TIME = 2.0f;

	// ----------------------------------------
	// 描画用定数
	// ----------------------------------------

	 // HPゲージ全体の横幅
	static constexpr int hpGaugeWidth = 200;

	// HPゲージ表示の座標
	static constexpr int ENEMY_HPGAUGE_OFFSET_X = -100;
	static constexpr int ENEMY_HPGAUGE_OFFSET_Y = -10;
	static constexpr int ENEMY_HPGAUGE_BOTTOM = 20;

	// HPゲージのY方向オフセット
	static constexpr float ENEMY_HPGAUGE_WORLD_OFFSET_Y = 40.0f;

	// HP半分以下でゲージを黄色にする
	static constexpr float HP_GAUGE_YELLOW_VALUE = 0.5f;

	// ----------------------------------------
	// 当たり判定
	// ----------------------------------------

	EnemyAttackHitBox hitBox;

	// 攻撃判定の半径
	float attackRadius = -1.0f;
	//float SKELETON_ATTACK_RADIUS = 10.0f;

	// 体の半径
	float bodyRadius = -1.0f;
	//float ENEMY_BODY_RADIUS = 0.0f;;

	// ----------------------------------------
	// 無敵時間
	// ----------------------------------------

	bool isInvincible = false;
	float invincibleTimer = 0.0f;

	// ----------------------------------------
	// 死亡処理
	// ----------------------------------------

	float deathTimer = 0.0f;

	// ----------------------------------------
	// フラグ
	// ----------------------------------------

	 // オブジェクトが有効かどうか
	bool isActive = false;

	// ----------------------------------------
	// 目的地
	// ----------------------------------------

	float targetX = 0.0f;
	float targetZ = 0.0f;
	
	// 目的地を設定済みかどうか
	bool hasTargetPosition = false;

	// ----------------------------------------
	// 攻撃クールタイム
	// ----------------------------------------

	float isAttackCoolTimer = 0.0f;

	// ----------------------------------------
	// モデル・描画
	// ----------------------------------------

	int modelHandle = -1;

	// ----------------------------------------
	// アニメーション
	// ----------------------------------------

    // アタッチするアニメーションのインデックス	int attachIndex = -1;
	float animTime = 0.0f;

	// アニメーションの再生時間
	int attachIndex = -1;

	 // アニメーションの総再生時間
	float totalTime = 0.0f;

	// 現在のアニメーション番号を保持
	int currentAnim = -1;

	// 各子クラスで自分専用のテーブルをセット
	const EnemyAnimData* animTable = nullptr;

	// ----------------------------------------
	// 乱数
	// ----------------------------------------

	// 乱数エンジン
	std::mt19937 rng;
};