#pragma once
#include "DxLib.h"

class EnemyWeapon {
public:

	EnemyWeapon();
	~EnemyWeapon();

	void SetUp(int srcModelHandle, int texHandle, const char* boneName);

	void Update(int enemyModelHandle);
	void Draw() const;

	VECTOR GetPos() const { return weaponPos; }

private:

	int weaponHandle = -1;
	int handFrameIndex = -1;

	float scale = 0.01f;
	float tipOffset = 180.0f;

	VECTOR weaponPos = VGet(0, 0, 0);

};