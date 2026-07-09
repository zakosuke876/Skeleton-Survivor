#pragma once
#include "Item.h"

// 効果音の種類
enum SoundType {
	RECOVERY,    // 回復
	EXP,         // 経験値取得
	LEVELUP,     // レベルアップ
	ATTACK_HIT,  // ヒットオン
	SE_MAX,      // 配列サイズ用
};

// BGMの種類
enum BgmType {
	BGM_TITLE,    // タイトル
	BGM_PLAY,     // ゲームプレイ中
	BGM_GAMEOVER, // ゲームオーバー
	BGM_MAX,      // 配列サイズ用
};

class SoundManager {
public:

	SoundManager();
	~SoundManager();

	// 指定した種類の効果音を再生
	void PlaySe(SoundType type) const;

	// 指定した種類のBGMを再生(すでに再生中なら処理しない)
	void PlayBgm(BgmType type)  const;

	// 指定した種類のBGMを停止
	void StopBgm(BgmType type)  const;

private:

	// 効果音ハンドルを保持する配列
	int seHandles[SE_MAX];

	// BGMハンドルを保持する配列
	int bgmHandles[BGM_MAX];

	// SE音量
	static constexpr int SE_VOLUME = 180;

	// BGM音量
	static constexpr int BGM_VOLUME = 128;
};