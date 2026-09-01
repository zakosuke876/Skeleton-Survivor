#include "ScoreManager.h"
#include "FontManager.h"
#include "DxLib.h"

ScoreManager::ScoreManager() {

	Reset();
}

void ScoreManager::Reset() {

	score = 0;

	// 経過時間・タイマーをリセット
	playTime = 0.0f;
	minutes = 0;
	seconds = 0;

	// 配列の値を0にリセット
	for (int i = 0; i < ENEMY_TYPE_COUNT; i++)
	{
		scoreByEnemyType[i] = 0;
	}
}

void ScoreManager::AddScore(EnemyType type, int value) {

	score += value;
	scoreByEnemyType[type] += value;
}

void ScoreManager::Update(float deltaTime) {

	playTime += deltaTime;

	// 分:秒に計算する
	int totalSeconds = (int)playTime;
	minutes = totalSeconds / 60;
	seconds = totalSeconds % 60;
}

void ScoreManager::Draw(const FontManager& fontManager) const {

	DrawFormatStringToHandle(SCORE_TEXT_X, SCORE_TEXT_Y, GetColor(255, 0, 0), fontManager.GetScoreFont(), "Score %06d", score);
	DrawFormatStringToHandle(TIME_TEXT_X, TIME_TEXT_Y, GetColor(255, 0, 0), fontManager.GetScoreFont(), "TIME %02d:%02d秒", minutes, seconds);
}