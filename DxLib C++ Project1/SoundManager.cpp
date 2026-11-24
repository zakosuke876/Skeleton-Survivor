#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager() {

	for (int i = 0; i < static_cast<int>(SoundType::SeMax); i++)
	{
		// タイプごとに効果音を読み込む
		seHandles[i] = LoadSoundMem(sePaths[i]);

		// 音量調整
		ChangeVolumeSoundMem(SE_VOLUME, seHandles[i]);
	}

	for (int i = 0; i < static_cast<int>(BgmType::BgmMax); i++)
	{
		// タイプごとにBGMを読み込む
		bgmHandles[i] = LoadSoundMem(bgmPaths[i]);

		ChangeVolumeSoundMem(BGM_VOLUME, bgmHandles[i]);
	}
}

SoundManager::~SoundManager() {

	// リソース開放
	for (int i = 0; i < static_cast<int>(SoundType::SeMax); i++)
	{
		DeleteSoundMem(seHandles[i]);
	}

	for (int i = 0; i < static_cast<int>(BgmType::BgmMax); i++)
	{
		DeleteSoundMem(bgmHandles[i]);
	}
}

void SoundManager::PlaySe(SoundType type) const {

	// タイプに応じた効果音を再生
	PlaySoundMem(seHandles[static_cast<int>(type)], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBgm(BgmType type) const {

	// すでに再生されていれば処理しない
	if (CheckSoundMem(bgmHandles[static_cast<int>(type)]) == 1) return;

	// BGMTypeがゲームオーバーの場合ループ再生しない
	if (type == BgmType::GameOver)
	{
		// ループ再生しない
		PlaySoundMem(bgmHandles[static_cast<int>(type)], DX_PLAYTYPE_BACK);
	}
	else
	{
		// ループ再生する
		PlaySoundMem(bgmHandles[static_cast<int>(type)], DX_PLAYTYPE_LOOP);
	}
}

void SoundManager::StopBgm(BgmType type) const {

	StopSoundMem(bgmHandles[static_cast<int>(type)]);
}
