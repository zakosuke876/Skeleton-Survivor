#pragma once
#include "GameConfig.h"

class Ground {
public:
	Ground();
	void Update();
	void DrawGround() const;

	static float GetMaxXPos() { return GameConfig::FIELD_MAX_X; }
	static float GetMinXPos() { return GameConfig::FIELD_MIN_X; }
	static float GetMaxZPos() { return GameConfig::FIELD_MAX_Z; }
	static float GetMinZPos() { return GameConfig::FIELD_MIN_Z; }
private:
	
	//　グリッドの設定
	// Y座標
	static constexpr float GROUND_Y = -3.6f;

	// 床のモデル
	int groundHandle;

	// 床の拡大倍率
	static constexpr float GROUND_SCALE = 0.25f;
};
