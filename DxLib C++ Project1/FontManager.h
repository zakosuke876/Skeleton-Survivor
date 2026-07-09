#pragma once

class FontManager {
public:

	FontManager();
	~FontManager();

	// プレイヤーのUI情報の表示で使うフォント
	int GetPlayerUIFont() const { return playerUIFont; }

	// ゲームのタイトル表示で使うフォント
	int GetTitleFont() const { return titleFont; }

	// ランキング情報の表示で使うフォント
	int GetRankingFont() const { return rankingFont; }

	// スコア表示で使うフォント
	int GetScoreFont() const { return scoreFont; }

	// ポーズ画面の表示で使うフォント
	int GetPauseFont() const { return pauseFont; }

private:

	// フォントハンドル
	int playerUIFont = -1;
	int titleFont    = -1;
	int rankingFont  = -1;
	int scoreFont    = -1;
	int manualFont   = -1;
	int pauseFont    = -1;

	// フォント名
	static constexpr const char* FONT_NAME = "Dungeon Mode";
};