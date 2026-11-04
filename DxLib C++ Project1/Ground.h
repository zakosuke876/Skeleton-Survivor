#pragma once
#include <array>
#include "GameConfig.h"

class Ground {
public:

	Ground();
	void DrawGround() const;
	float GetSpeedMultiplierAtWorldPos(float worldX, float worldZ) const;
	unsigned int GetGroundEffectColor(float worldX, float worldZ) const;

	static float GetMaxXPos() { return GameConfig::FIELD_MAX_X; }
	static float GetMinXPos() { return GameConfig::FIELD_MIN_X; }
	static float GetMaxZPos() { return GameConfig::FIELD_MAX_Z; }
	static float GetMinZPos() { return GameConfig::FIELD_MIN_Z; }

private:

	GameConfig::GroundType GetType(int x, int z) const;
	void SetType(int x, int z, GameConfig::GroundType type);
	bool IsOutOfRange(int pos) const;
	int ToIndex(int pos) const;
	GameConfig::GroundType GetTypeAtWorldPos(float worldX, float worldZ) const;

	// マスごとの地形タイプ(GRID_WIDTH * GRID_WIDTH) 見た目と効果の判定に使う
	std::array<std::array<GameConfig::GroundType, GameConfig::GRID_WIDTH>, GameConfig::GRID_WIDTH> grids{};

	// GameConfigの enum GroundType と宣言順を合わせる
	static constexpr float speedMultiplierByType[static_cast<int>(GameConfig::GroundType::MAX)] = {
		1.0f,  // Normal
		0.8f,  // Grass
		0.6f,  // Water
		0.3f,  // Sand
	};

	// 床を配置するY座標
	static constexpr float GROUND_Y = -3.6f;

	// 床のモデル
	int groundHandle = -1;

	// 通常床テクスチャ
	int groundNormalHandle = -1;

	// 草テクスチャ
	int groundGrassHandle = -1;

	// 水テクスチャ
	int groundWaterHandle = -1;

	// 砂テクスチャ
	int groundSandHandle = -1;

	// 床の拡大倍率
	static constexpr float GROUND_SCALE = 0.25f;
};
