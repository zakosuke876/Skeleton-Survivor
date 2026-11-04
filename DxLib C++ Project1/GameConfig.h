#pragma once

namespace GameConfig {
	constexpr int   GRID_HALF_NUM = 10; // 片側のマス数(地面)
	constexpr float GRID_SIZE = 50.0f;  // 1マスのサイズ(地面)

	enum class GroundType {
		Normal, // 通常
		Grass,  // 草地帯
		Water,  // 水地帯
		Sand,   // 砂地帯
		MAX,    
	};

	// 片側10 * 2 + 中央1
	constexpr int GRID_WIDTH = GRID_HALF_NUM * 2 + 1;

	// ウィンドウ設定
	constexpr int SCREEN_WIDTH = 1280;
	constexpr int SCREEN_HEIGHT = 720;

	// 各座標の最大・最小値(フィールドの範囲)
	constexpr float FIELD_MIN_X  = -GRID_HALF_NUM * GRID_SIZE;
	constexpr float FIELD_MAX_X  =  GRID_HALF_NUM * GRID_SIZE;
	constexpr float FIELD_MIN_Z  = -GRID_HALF_NUM * GRID_SIZE;
	constexpr float FIELD_MAX_Z  =  GRID_HALF_NUM * GRID_SIZE;
}
