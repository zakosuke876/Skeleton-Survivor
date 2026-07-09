#include "FontManager.h"
#include "DxLib.h"

FontManager::FontManager() {

	// dungeon-modeを使うフォント
	AddFontResourceEx("Font/dungeon-mode.ttf", FR_PRIVATE, nullptr);
	playerUIFont = CreateFontToHandle(FONT_NAME, 15, 3);
	titleFont    = CreateFontToHandle(FONT_NAME, 30, 3);
	scoreFont    = CreateFontToHandle(FONT_NAME, 16, 3);
	rankingFont  = CreateFontToHandle(FONT_NAME, 40, 3);

	// 日本語表示用の標準フォント
	manualFont   = CreateFontToHandle(NULL, 30, 3);
	pauseFont    = CreateFontToHandle(NULL, 30, 3);
}

FontManager::~FontManager() {

	// リソース開放
	DeleteFontToHandle(playerUIFont);
	DeleteFontToHandle(titleFont);
	DeleteFontToHandle(scoreFont);
	DeleteFontToHandle(manualFont);
	DeleteFontToHandle(rankingFont);
	DeleteFontToHandle(pauseFont);
	RemoveFontResourceEx("Font/dungeon-mode.ttf", FR_PRIVATE, nullptr);
}
