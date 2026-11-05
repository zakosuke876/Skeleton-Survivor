#include "Random.h"

Random::Random() {

	std::random_device rd;
	rng = std::mt19937(rd());
}

int Random::GetRandomInt(int min, int max) {

	std::uniform_int_distribution<int> intDist(min, max);

	int value = intDist(rng);

	return value;
}

float Random::GetRandomFloat(float min, float max) {

	std::uniform_real_distribution<float> floatDist(min, max);

	float value = floatDist(rng);

	return value;
}
