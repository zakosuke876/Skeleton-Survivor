#pragma once

// 前方宣言
class EnemyManager;
class ScoreManager;
class RankingManager;
class FontManager;

class Result {
public:

	Result();
	~Result();

	void Draw(EnemyManager& enemyManager, ScoreManager& scoreManager, RankingManager& rankingManager, FontManager& fontManager) const;

private:

	void DrawPanel() const;
	void DrawEnemyInfo(EnemyManager& enemyManager, ScoreManager& scoreManager) const;
	void DrawPlayResult(ScoreManager& scoreManager) const;
	const char* GetRank(int score) const;

	// 画像ハンドル
	int skeletonNIcon = -1;
	int skeletonWIcon = -1;
	int skeletonMIcon = -1;

	// フォントハンドル
	int headerFont = -1;
	int enemyInfoFont = -1;
	int textFont = -1;

	// リザルトパネル
	// (見出し)
	static constexpr int RESULT_HEADER_LEFT = 500;
	static constexpr int RESULT_HEADER_TOP = 30;
	static constexpr int RESULT_HEADER_RIGHT = 800;
	static constexpr int RESULT_HEADER_BOTTOM = 100;

	// リザルト情報パネル
	static constexpr int RESULT_PANEL_LEFT = 100;
	static constexpr int RESULT_PANEL_TOP = 100;
	static constexpr int RESULT_PANEL_RIGHT = 1180;
	static constexpr int RESULT_PANEL_BOTTOM = 600;

	// 敵キャラ
	static constexpr int RESULT_ENEMY_IMAGE_LEFT = 100;
	static constexpr int RESULT_ENEMY_IMAGE_TOP = 200;
	static constexpr int RESULT_ENEMY_IMAGE_RIGHT = 250;
	static constexpr int RESULT_ENEMY_IMAGE_BOTTOM = 350;

	// 2体目のずらし幅
	static constexpr int RESULT_ENEMY_IMAGE_OFFSET_WARRIOR = 200;

	// 3体目のずらし幅
	static constexpr int RESULT_ENEMY_IMAGE_OFFSET_MAGE = 400;

	// 敵ごとの情報を表示する座標
	static constexpr int RESULT_TEXT_NORMAL_X = 100 + 20;
	static constexpr int RESULT_TEXT_WARRIOR_X = 300 + 20;
	static constexpr int RESULT_TEXT_MAGE_X = 500 + 20;

	// 倒した数、倒して得たスコア
	static constexpr int RESULT_TEXT_KILL_Y = 350 + 10;
	static constexpr int RESULT_TEXT_SCORE_Y = 350 + 10 + 50;

	// ランク判定
	static constexpr int RANK_S_SCORE = 5000;
	static constexpr int RANK_A_SCORE = 4000;
	static constexpr int RANK_B_SCORE = 3000;
	static constexpr int RANK_C_SCORE = 2000;
};
