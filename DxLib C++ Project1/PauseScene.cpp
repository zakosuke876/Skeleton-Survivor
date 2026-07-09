#include "PauseScene.h"
#include "FontManager.h"
#include "DxLib.h"

PauseScene::PauseScene() {

	Reset();
}

void PauseScene::Reset() {

	isPause = false;
}

void PauseScene::Update() {

	// 前フレームの状態をコピー
	memcpy(prevKey, currentKey, sizeof(currentKey));

	// 今フレームの状態で上書き
	GetHitKeyStateAll(currentKey);

	if (currentKey[KEY_INPUT_P] && !prevKey[KEY_INPUT_P])
	{
		isPause = !isPause;
	}
}

void PauseScene::Draw(const FontManager& fontManager) const {

	if (!isPause) return;

	// Pauseパネルヘッダー
	DrawBox(PAUSE_HEADER_BOX_LEFT, PAUSE_HEADER_BOX_TOP, PAUSE_HEADER_BOX_RIGHT, PAUSE_HEADER_BOX_BOTTOM, GetColor(30, 30, 30), TRUE);
	DrawBox(PAUSE_HEADER_BOX_LEFT, PAUSE_HEADER_BOX_TOP, PAUSE_HEADER_BOX_RIGHT, PAUSE_HEADER_BOX_BOTTOM, GetColor(0, 0, 255), FALSE); // 枠線
	DrawStringToHandle(PAUSE_HEADER_TEXT_X, PAUSE_HEADER_TEXT_Y, "ポーズ中…", GetColor(255, 0, 0), fontManager.GetPauseFont());

	// 情報パネル(半透明)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(PAUSE_INFO_BOX_LEFT, PAUSE_INFO_BOX_TOP, PAUSE_INFO_BOX_RIGHT, PAUSE_INFO_BOX_BOTTOM, GetColor(30, 30, 30), TRUE);

	DrawStringToHandle(480, PAUSE_CLOSE_TEXT_Y, "Pキー : ゲームに戻る", GetColor(255, 0, 0), fontManager.GetPauseFont());
	DrawStringToHandle(480, PAUSE_TITLE_BACK_TEXT_Y, "Rキー : タイトルに戻る", GetColor(255, 0, 0), fontManager.GetPauseFont());

	// ブレンド解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawBox(PAUSE_INFO_BOX_LEFT, PAUSE_INFO_BOX_TOP, PAUSE_INFO_BOX_RIGHT, PAUSE_INFO_BOX_BOTTOM, GetColor(0, 0, 255), FALSE); // 情報パネルの枠線
}