#pragma once

class MagicEffect {
public:

	MagicEffect();
	~MagicEffect();

	void Update(float deltaTime);
	void Draw() const;

private:

	float x = 0.0f;
	float y = 20.0f;
	float z = 0.0f;

	static constexpr int EFFECT_ALL = 11;
	static constexpr int EFFECT_DIV_X = 11;
	static constexpr int EFFECT_DIV_Y = 1;
	static constexpr int EFFECT_WIDTH = 48;
	static constexpr int EFFECT_HEIGHT = 48;

	int grHandles[EFFECT_ALL] = {};

	static constexpr int BOLT_START = 0;
	static constexpr int BOLT_END   = 3;

	static constexpr int IMPACT_START = 5;
	static constexpr int IMPACT_END = 10;

	int startFrame = BOLT_START;
	int endFrame = BOLT_END;
	bool isLoop = true;

	int currentFrame = BOLT_START;
	// コマ送り用タイマー
	float frameTimer = 0.0f;

	// 1コマの表示時間(秒)
	static constexpr float FRAME_INTERVAL = 1.0f / 15.0f;

	// 描画時の拡大倍率
	static constexpr float EFFECT_SCALE = 50.0f;
};