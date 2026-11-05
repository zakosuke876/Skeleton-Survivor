#pragma once
#include <random>


class Random {
public:

	Random();
	int GetRandomInt(int min, int max);
	float GetRandomFloat(float min, float max);

private:

	// 乱数エンジン
	std::mt19937 rng;
};
