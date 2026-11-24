#pragma once

// 効果音の種類
enum class SoundType {
	Recovery,  // 回復
	Exp,       // 経験値取得
	LevelUp,   // レベルアップ
	AttackHit, // ヒット音
	SeMax,     // 配列サイズ用
};

// BGMの種類
enum class BgmType {
	Title,    // タイトル
	Play,     // ゲームプレイ中
	GameOver, // ゲームオーバー
	BgmMax,      // 配列サイズ用
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
	int seHandles[static_cast<int>(SoundType::SeMax)];

	// 各効果音のパスをまとめた配列
	// 並び順は SoundType の定義順と一致させる
	static constexpr const char* sePaths[static_cast<int>(SoundType::SeMax)] = {
		"Sound/SE/Recovery.mp3",
		"Sound/SE/Exp.mp3",
		"Sound/SE/LevelUp.mp3",
		"Sound/SE/Attack_Hit.mp3",
	};

	// BGMハンドルを保持する配列
	int bgmHandles[static_cast<int>(BgmType::BgmMax)];
	
	// 各BGMのパスをまとめた配列
	// 並び順は BgmType の定義順と一致させる
	static constexpr const char* bgmPaths[static_cast<int>(BgmType::BgmMax)] = {
		"Sound/BGM/Title_BGM.mp3",
		"Sound/BGM/Play_BGM.mp3",
		"Sound/BGM/GameOver_BGM.mp3",
	};

	// SE音量
	static constexpr int SE_VOLUME = 180;

	// BGM音量
	static constexpr int BGM_VOLUME = 128;
};
