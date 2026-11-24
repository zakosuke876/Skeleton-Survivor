#pragma once
#include "AnimationClip.h"
#include "DxLib.h"

class Effect {
public:

	Effect() = default;
	void Play(VECTOR pos, const AnimationClip& animClip, float delayTime = 0.0f);
	void Update(float deltaTime);
	void Draw() const;

	bool IsActive() const { return isActive; }

	// 表示座標を外部から設定
	void SetPosition(VECTOR pos) {position = pos; }

private:

	AnimationClip clip;

	VECTOR position;

	// 現在のコマ
	int currentFrame = 0;

	float frameTimer = 0.0f;

	bool isActive = false;

	float delayTimer = 0.0f;
};
