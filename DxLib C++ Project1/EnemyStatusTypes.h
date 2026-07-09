#pragma once


// ここに敵固有のステータスを構造体で定義してJSONからデータを読み込む

// JSONから読み込んだウォリアーの固有ステータスを格納する構造体
// 読み込んだ値をSkeleton_Warriorクラスのメンバ変数へコピーして使用
struct SkeletonWarriorStatus {
	int reviveChancePercent;   // 復活確率
	float reviveWaitTime;      // 復活待機時間
	float reviveHpMultiplier;  // 復活した際のHp倍率
	int reviveScoreMultiplier; // 復活した際のスコア倍率
};