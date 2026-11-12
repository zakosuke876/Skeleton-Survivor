#pragma once


class CollisionSystem {
public:

	// 円同士の当たり判定
	bool IsCircleHit(float x1, float z1, float r1, float x2, float z2, float r2) const;
};
