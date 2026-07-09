#pragma once

// 前方宣言
class FontManager;

class PauseScene {
public:

	PauseScene();
	void Reset(); // ゲームリセット処理
	void Update();
	void Draw(const FontManager& fontManager) const;

	bool GetIsPause() const { return isPause; }

private:

	// ポーズ状態かどうか
	bool isPause = false;

	// ボタンの数
	static constexpr int MAX_BUTTON = 2;

	// キーの入力状態チェック
	char prevKey[256]{};
	char currentKey[256]{};

	// パネル[ポーズ中…]表示の座標
	static constexpr int PAUSE_HEADER_BOX_LEFT  = 500;
	static constexpr int PAUSE_HEADER_BOX_TOP   = 150;
	static constexpr int PAUSE_HEADER_BOX_RIGHT = 750;
	static constexpr int PAUSE_HEADER_BOX_BOTTOM = 200;

	// ヘッダーテキストの座標
	static constexpr int PAUSE_HEADER_TEXT_X = 560;
	static constexpr int PAUSE_HEADER_TEXT_Y = 160;

	// 情報パネルの座標
	static constexpr int PAUSE_INFO_BOX_LEFT = 350;
	static constexpr int PAUSE_INFO_BOX_TOP = 200;
	static constexpr int PAUSE_INFO_BOX_RIGHT = 900;
	static constexpr int PAUSE_INFO_BOX_BOTTOM = 600;

	// 情報パネル内テキストのX座標
	static constexpr int PAUSE_INFO_TEXT_X = 480;

	// 「ゲームに戻る」テキストのY座標
	static constexpr int PAUSE_CLOSE_TEXT_Y = 300;

	// 「タイトルに戻る」テキストのY座標
	static constexpr int PAUSE_TITLE_BACK_TEXT_Y = 450;
};