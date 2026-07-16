#pragma once
#include "DxLib.h"

class Sword {
public:

	Sword();
	~Sword();
	//void Reset(); // ゲームリセット処理

	void SetUpBone(int playerModelHandle);
	void Update(int playerModelHandle);
	void Draw() const;

	VECTOR GetPos() const { return swordPos; }

private:

	int swordHandle = -1;
	int swordTexHandle = -1;
	int handFrameIndex = -1;

	static constexpr float SWORD_SCALE = 0.01f;

	// 剣の柄から刃先までの距離
	static constexpr float SWORD_TIP_OFFSET = 180.0f;

	VECTOR swordPos = VGet(0, 0, 0);
};