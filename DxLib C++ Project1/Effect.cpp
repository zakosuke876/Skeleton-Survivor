#include "Effect.h"

Effect::Effect() {

}

void Effect::Play(VECTOR pos, const AnimationClip& animClip, float delayTime) {

	if (animClip.handles == nullptr || animClip.handleNum == 0) return;

	position = pos;
	clip = animClip;
	isActive = true;
	currentFrame = 0;
	frameTimer = 0.0f;
	delayTimer = delayTime;
}

void Effect::Update(float deltaTime) {

	if (!isActive) return;

	if (delayTimer > 0.0f)
	{
		delayTimer -= deltaTime;
		return;
	}

	// タイマーを進める
	frameTimer += deltaTime;

	// コマの表示時間を超えた場合
	if (frameTimer >= clip.frameTime)
	{
		frameTimer = 0.0f;

		// 次のコマに進める
		currentFrame++;

		// 最終コマまで再生した場合
		if (currentFrame >= clip.handleNum)
		{
			// 表示終了
			isActive = false;
		}
	}
}

void Effect::Draw() const {

	if (!isActive) return;

	if (delayTimer > 0.0f) return;

	// ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(position);

	DrawRotaGraph(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), 0.7f, 0.0f,
		clip.handles[currentFrame], TRUE);
}
