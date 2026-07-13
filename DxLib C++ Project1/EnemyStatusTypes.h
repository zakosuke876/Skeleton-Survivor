#pragma once
#include <string>


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


// ここから下に敵固有のステータスを構造体で定義してJSONからデータを読み込む

// ノーマルスケルトンのステータス構造体
struct SkeletonNormalStatus {
	EnemyStatus enemyStatus;
};

// ウォーリアースケルトンのステータス構造体
struct SkeletonWarriorStatus {
	EnemyStatus enemyStatus;
	int reviveChancePercent;   // 復活確率
	float reviveWaitTime;      // 復活待機時間
	float reviveHpMultiplier;  // 復活した際のHp倍率
	int reviveScoreMultiplier; // 復活した際のスコア倍率
};