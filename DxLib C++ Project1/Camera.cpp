#include "Camera.h"
#include <cmath>


void Camera::UpdateCamera(VECTOR playerPos) {

	VECTOR cameraPos;
	cameraPos.x = playerPos.x + sinf(angleY) * CAMERA_DISTANCE;
	cameraPos.y = playerPos.y + CAMERA_HEIGHT;
	cameraPos.z = playerPos.z - cosf(angleY) * CAMERA_DISTANCE;

	SetCameraPositionAndTarget_UpVecY(cameraPos, playerPos);
}