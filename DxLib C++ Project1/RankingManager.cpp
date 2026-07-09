#include "RankingManager.h"
#include "ScoreManager.h"
#include "FontManager.h"
#include "Dxlib.h"
#include <algorithm>

RankingManager::RankingManager() {

	Reset();
}

void RankingManager::Reset() {

	newRank = -1;
	blinkTimer = 0.0f;
	isBlinkVisible = false;
}

void RankingManager::SortRanking() {

	// ランキングソート(降順)
	std::stable_sort(ranking.begin(), ranking.end(), std::greater<int>());
}

void RankingManager::AddScore(const ScoreManager& scoreManager) {

	// 今回のスコアを保存
	int score = scoreManager.GetScore();

	// スコアをランキングに追加
	// 同値は先に登録されたスコアが上位になる
	ranking.emplace_back(score);

	SortRanking();

	// 件数が上限を超えた場合
	if (ranking.size() > MAX_RANKING)
	{
		// 上位5件にする
		ranking.resize(MAX_RANKING);
	}

	// 順位を調べる
	for (size_t i = 0; i < MAX_RANKING; i++)
	{
		if (score == ranking[i])
		{
			newRank = i + 1;
			break;
		}
	}
}

void RankingManager::Update(float deltaTime) {

	// 記録更新がされなかった場合は点滅処理を行わない
	if (newRank == -1) return;

	blinkTimer += deltaTime;

	// 点滅間隔を超えた場合
	if (blinkTimer >= BLINK_INTERVAL)
	{
		isBlinkVisible = !isBlinkVisible;

		blinkTimer = 0.0f;
	}
}

void RankingManager::DrawRanking(const FontManager& fontManager) const {

	for (size_t i = 0; i < ranking.size(); i++)
	{
		if (static_cast<int>(i) + 1 == newRank && isBlinkVisible) continue;

		// ランキングスコアを表示
		DrawFormatStringToHandle(RANKING_X, RANKING_Y + i * RANKING_LINE_HEIGHT, GetColor(255, 0, 0), fontManager.GetRankingFont(), "%d位:%d", i + 1, ranking[i]);
	}
}

void RankingManager::DrawTitleRanking(const FontManager& fontManager) const {

	// ランキング表示パネル
	DrawBox(TITLE_RANKING_BOX_LEFT, TITLE_RANKING_BOX_TOP, TITLE_RANKING_BOX_RIGHT, TITLE_RANKING_BOX_BOTTOM, GetColor(30, 30, 30), TRUE);
	DrawStringToHandle(TITLE_RANKING_TEXT_X, TITLE_RANKING_TEXT_Y, "RANKING", GetColor(255, 0, 0), fontManager.GetRankingFont());

	for (size_t i = 0; i < ranking.size(); i++)
	{

		// ランキングスコアを表示
		DrawFormatStringToHandle(TITLE_RANKING_X, TITLE_RANKING_Y + i * TITLE_RANKING_LINE_HEIGHT, GetColor(255, 0, 0), fontManager.GetRankingFont(), "%d位:%d", i + 1, ranking[i]);
	}
}
