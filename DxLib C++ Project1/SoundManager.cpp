#include "SoundManager.h"
#include "DxLib.h"



SoundManager::SoundManager() {

	// タイプごとに効果音を読み込む
	seHandles[RECOVERY]   = LoadSoundMem("Sound/SE/Recovery.mp3");
	seHandles[EXP]        = LoadSoundMem("Sound/SE/Exp.mp3");
	seHandles[LEVELUP]    = LoadSoundMem("Sound/SE/LevelUp.mp3");
	seHandles[ATTACK_HIT] = LoadSoundMem("Sound/SE/Attack_Hit.mp3");

	// タイプごとにBGMを読み込む
	bgmHandles[BGM_TITLE] = LoadSoundMem("Sound/BGM/Title_BGM.mp3");
	bgmHandles[BGM_PLAY] = LoadSoundMem("Sound/BGM/Play_BGM.mp3");
	bgmHandles[BGM_GAMEOVER] = LoadSoundMem("Sound/BGM/GameOver_BGM.mp3");

	// 音量調整
	for (int i = 0; i < BGM_MAX; i++)
	{
		ChangeVolumeSoundMem(BGM_VOLUME, bgmHandles[i]);
	}

	for (int i = 0; i < SE_MAX; i++)
	{
		ChangeVolumeSoundMem(SE_VOLUME, seHandles[i]);
	}
}

SoundManager::~SoundManager() {

	// リソース開放
	for (int i = 0; i < SE_MAX; i++)
	{
		DeleteSoundMem(seHandles[i]);
	}

	for (int i = 0; i < BGM_MAX; i++)
	{
		DeleteSoundMem(bgmHandles[i]);
	}
}

void SoundManager::PlaySe(SoundType type) const {

	// タイプに応じた効果音を再生
	PlaySoundMem(seHandles[type], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBgm(BgmType type) const {

	// すでに再生されていれば処理しない
	if (CheckSoundMem(bgmHandles[type]) == 1) return;

	// BGMTypeがゲームオーバーの場合ループ再生しない
	if (type == BGM_GAMEOVER)
	{
		// ループ再生しない
		PlaySoundMem(bgmHandles[type], DX_PLAYTYPE_BACK);
	}
	else
	{
		// ループ再生する
		PlaySoundMem(bgmHandles[type], DX_PLAYTYPE_LOOP);
	}
}

void SoundManager::StopBgm(BgmType type) const {

	StopSoundMem(bgmHandles[type]);
}