#pragma once
#include "DxLib.h"

class Camera {
public:

	void UpdateCamera(VECTOR playerPos);

private:

	// カメラの位置(Y座標)
	static constexpr float CAMERA_HEIGHT = 130.0f;

	// カメラの位置(Z座標)
	static constexpr float CAMERA_DISTANCE = 200.0f;

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	float angleY = 0.0f;
};