#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager() {

	// タイプごとに効果音を読み込む
	seHandles[static_cast<int>(SoundType::Recovery)]   = LoadSoundMem("Sound/SE/Recovery.mp3");
	seHandles[static_cast<int>(SoundType::Exp)]        = LoadSoundMem("Sound/SE/Exp.mp3");
	seHandles[static_cast<int>(SoundType::LevelUp)]    = LoadSoundMem("Sound/SE/LevelUp.mp3");
	seHandles[static_cast<int>(SoundType::AttackHit)] = LoadSoundMem("Sound/SE/Attack_Hit.mp3");

	// タイプごとにBGMを読み込む
	bgmHandles[static_cast<int>(BgmType::Title)] = LoadSoundMem("Sound/BGM/Title_BGM.mp3");
	bgmHandles[static_cast<int>(BgmType::Play)] = LoadSoundMem("Sound/BGM/Play_BGM.mp3");
	bgmHandles[static_cast<int>(BgmType::GameOver)] = LoadSoundMem("Sound/BGM/GameOver_BGM.mp3");

	// 音量調整
	for (int i = 0; i < static_cast<int>(BgmType::BgmMax); i++)
	{
		ChangeVolumeSoundMem(BGM_VOLUME, bgmHandles[i]);
	}

	for (int i = 0; i < static_cast<int>(SoundType::SeMax); i++)
	{
		ChangeVolumeSoundMem(SE_VOLUME, seHandles[i]);
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
