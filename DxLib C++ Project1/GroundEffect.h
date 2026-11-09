#pragma once
#include <array>
#include <random>

class GroundEffect {
public:

	GroundEffect();
	void Spawn(float startX, float startZ, unsigned int effectColor);
	void Update(float deltaTime);
	void Draw() const;

private:

	// エフェクト1つが持つデータ
	struct Particle {
		float x = 0.0f;          // 生成座標
		float y = 0.0f;          // 生成座標
		float z = 0.0f;          // 生成座標
		float vX = 0.0f;         // 飛散速度
		float vY = 0.0f;         // 飛散速度
		float vZ = 0.0f;         // 飛散速度
		float lifeTime = 0.0f;   // 表示時間
		float timer = 0.0f;      // 寿命計測タイマー
		bool isActive = false;   // 有効かどうか
		unsigned int color = 0;  // 描画するエフェクトの色
	};

	// エフェクトの最大同時出現数
	static constexpr int MAX_PARTICLE_COUNT = 10;

	// エフェクトのプール(使いまわす)
	std::array<Particle, MAX_PARTICLE_COUNT> particles{};

	// 乱数エンジン
	std::mt19937 rng;

	// 飛散速度の乱数範囲
	static constexpr float MIN_RAND_VECTOR = -100.0f;
	static constexpr float MAX_RAND_VECTOR =  100.0f;

	static constexpr float MIN_RAND_Y_VECTOR = 100.0f;
	static constexpr float MAX_RAND_Y_VECTOR = 200.0f;
	
	// エフェクトの生存時間
	static constexpr float PARTICLE_LIFETIME = 0.15f;

	// 生成間隔
	static constexpr float PARTICLE_INTERVAL = 0.1f;
	float spawnTimer = 0.0f;

	// 重力
	static constexpr float GRAVITY = 1000.0f;

	// 球の見た目
	static constexpr float PARTICLE_RADIUS = 1.0f;
	static constexpr int PARTICLE_DIV_NUM = 8;
};
