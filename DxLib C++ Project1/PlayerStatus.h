#pragma once
#include <string>

// JSONから読み込んだプレイヤーの設定値(ステータス・当たり判定・リソースパスなど)を格納する構造体
// 読み込んだ値をPlayerクラスのメンバ変数へコピーして使用
struct PlayerStatus {
	int maxHp;                    // 最大HP
	int attackPower;              // 攻撃力
	float speed;                  // 歩きスピード
	float dashMultiplier;         // ダッシュ時のスピード倍率
	int startLevel;               // 初期レベル
	int startNextLevelExp;        // 次のレベルアップに必要な経験値の初期値
	float attackRadius;           // 攻撃判定の半径
	float bodyRadius;             // 体の当たり判定半径
	int levelUpAttackPower;       // レベルアップで上昇する攻撃力
	float nextLevelExpMultiplier; // レベルアップに必要な経験値の倍率
	float invincibleTime;         // 無敵時間
	float hideTime;               // 死亡後に消えるまでの時間
	float levelUpTextActiveTime;  // レベルアップ演出(テキスト)の表示時間
	float scale;                  // モデルの拡大倍率
	std::string modelPath;        // モデルファイルのパス
	std::string texturePath;      // テクスチャファイルのパス
};