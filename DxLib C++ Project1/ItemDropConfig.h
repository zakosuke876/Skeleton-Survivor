#pragma once

struct ItemTypeRate {
	int smallRate;  // 小タイプ
	int mediumRate; // 中タイプ
	int largeRate;  // 大タイプ
};

struct ItemDropConfig {
	int dropRate;              // アイテムのドロップ率(%)
	float dropOffsetY;         // ドロップ時の座標をずらすためのY方向オフセット
	float expItemRandomOffset; // 経験値アイテムの生成座標をずらす範囲
	ItemTypeRate itemTypeRate;
};