#include "CollisionSystem.h"

bool CollisionSystem::IsCircleHit(float x1, float z1, float r1, float x2, float z2, float r2) const {

	float dx = x2 - x1;
	float dz = z2 - z1;

	float distanceSq = dx * dx + dz * dz;

	float totalRadius = r1 + r2;

	return (distanceSq < totalRadius * totalRadius);
}
