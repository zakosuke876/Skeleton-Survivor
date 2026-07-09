#pragma once
#include "EnemyType.h"

// 前方宣言
class FontManager;

class ScoreManager {
public:

	ScoreManager();
	void Reset(); // ゲームリセット処理
	void AddScore(EnemyType type, int value);
	void Update(float deltaTime);
	void Draw(const FontManager& fontManager) const;

	// 得たスコアを敵の種類別で取得する
	int GetScoreByEnemyType(EnemyType type) const { return scoreByEnemyType[type]; }

	// 総スコアを取得する
	int GetScore() const { return score; }

	// 生存時間を取得(分・秒)
	int GetMinutes() const { return minutes; }
	int GetSeconds() const { return seconds; }

private:

	int score = 0;

	// プレイ時間を計測(分・秒で表示)
	float playTime = 0.0f;
	int minutes = 0;
	int seconds = 0;

	// 敵の種類ごとにスコアを加算する配列
	int scoreByEnemyType[ENEMY_TYPE_COUNT] = {0};

	// スコア表示の座標
	static constexpr int SCORE_TEXT_X  = 0;
	static constexpr int SCORE_TEXT_Y = 20;

	// 生存時間表示の座標
	static constexpr int TIME_TEXT_X = 0;
	static constexpr int TIME_TEXT_Y = 60;
};