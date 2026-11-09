#pragma once

class InputManager {
public:

	void Update();

	// 押されている間 true
	bool IsDown(int keyCode) const;

	// 押した瞬間だけ true
	bool IsPressed(int keyCode) const;


private:

	// キーの総数
	static constexpr int KEY_COUNT = 256;

	// 前のフレームと比較して押した瞬間を判定する
	char prevKey[KEY_COUNT]{};
	char currentKey[KEY_COUNT]{};
};
