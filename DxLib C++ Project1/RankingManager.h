#pragma once
#include <vector>

// 前方宣言
class ScoreManager;
class FontManager;

class RankingManager {
public:

	RankingManager();
	void Reset(); // ゲームリセット処理

	// 現在のスコアをランキングへ登録する
	void AddScore(const ScoreManager& scoreManager);
	void Update(float deltaTime);

	// 更新した順位を取得する
	int GetNewRank() const { return newRank; }
	void DrawRanking(const FontManager& fontManager) const;

	// タイトル画面用
	void DrawTitleRanking(const FontManager& fontManager) const;

private:

	// ランキングをソートする(降順)
	void SortRanking();

	// ランキングの最大保持数
	static constexpr int MAX_RANKING = 5;

	// 初期ランキング(降順)
	std::vector<int> ranking{ 5000, 4000, 3000, 2000, 1000 };

	// 記録更新した順位を保持
	int newRank = -1;

	// ランキング更新時にテキストを点滅させる
	float blinkTimer = 0.0f;

	// 点滅間隔(秒)
	static constexpr float BLINK_INTERVAL = 0.5f;

	// テキストを点滅させるかどうか
	bool isBlinkVisible = false;

	// ランキング表示座標(ゲームオーバー)

	 // ランキング表示座標
	static constexpr int RANKING_X = 820;
	static constexpr int RANKING_Y = 230;

	// ランキングの高さ間隔
	static constexpr int RANKING_LINE_HEIGHT = 60;

	// ランキング表示座標(タイトルシーン)
	 // ランキング表示パネル左端
	static constexpr int TITLE_RANKING_BOX_LEFT   = 450;

	// ランキング表示パネル上端
	static constexpr int TITLE_RANKING_BOX_TOP    = 100;

	// ランキング表示パネル右端
	static constexpr int TITLE_RANKING_BOX_RIGHT  = 850;

	// ランキング表示パネル下端
	static constexpr int TITLE_RANKING_BOX_BOTTOM = 650;

	// RANKINGテキスト表示座標
	static constexpr int TITLE_RANKING_TEXT_X     = 500;
	static constexpr int TITLE_RANKING_TEXT_Y     = 150;

	// ランキング表示座標
	static constexpr int TITLE_RANKING_X = 530;
	static constexpr int TITLE_RANKING_Y = 230;

	// ランキングの高さ間隔
	static constexpr int TITLE_RANKING_LINE_HEIGHT = 80;
};