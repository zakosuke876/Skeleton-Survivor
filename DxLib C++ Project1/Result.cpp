#include "Result.h"
#include "DxLib.h"
#include "EnemyManager.h"
#include "ScoreManager.h"
#include "RankingManager.h"


Result::Result() {

	// フォント作成
    headerFont = CreateFontToHandle("Dungeon Mode", 30, 3);
    enemyInfoFont = CreateFontToHandle("Dungeon Mode", 16, 3);
    textFont = CreateFontToHandle("Dungeon Mode", 30, 3);

    // 敵画像読み込み
    skeletonIcon = LoadGraph("UI/Enemy/Skeleton_W_Icon - コピー.png");
    skeletonNIcon = LoadGraph("UI/Enemy/Skeleton_N_Icon - コピー.png");
}

Result::~Result() {

	// リソース開放
    DeleteFontToHandle(headerFont);
    DeleteFontToHandle(enemyInfoFont);
    DeleteFontToHandle(textFont);

    DeleteGraph(skeletonIcon);
    DeleteGraph(skeletonNIcon);
}

void Result::DrawPanel() const {

    // GAMEOVERの見出し
    DrawBox(RESULT_HEADER_LEFT, RESULT_HEADER_TOP, RESULT_HEADER_RIGHT, RESULT_HEADER_BOTTOM, GetColor(30, 30, 30), TRUE);
    DrawBox(RESULT_HEADER_LEFT, RESULT_HEADER_TOP, RESULT_HEADER_RIGHT, RESULT_HEADER_BOTTOM, GetColor(0, 0, 255), FALSE); // 枠線
    DrawFormatStringToHandle(520, 50, GetColor(255, 255, 255), headerFont, "GAME OVER");

    // 情報パネル(半透明)
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(RESULT_PANEL_LEFT, RESULT_PANEL_TOP, RESULT_PANEL_RIGHT, RESULT_PANEL_BOTTOM, GetColor(30, 30, 30), TRUE);

    // ブレンド解除
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    DrawBox(RESULT_PANEL_LEFT, RESULT_PANEL_TOP, RESULT_PANEL_RIGHT, RESULT_PANEL_BOTTOM, GetColor(0, 0, 255), FALSE); // 情報パネルの枠線
}

void Result::DrawEnemyInfo(EnemyManager& enemyManager, ScoreManager& scoreManager) const {

    // 敵の画像を表示
    DrawExtendGraph(RESULT_ENEMY_IMAGE_LEFT, RESULT_ENEMY_IMAGE_TOP, RESULT_ENEMY_IMAGE_RIGHT, RESULT_ENEMY_IMAGE_BOTTOM, skeletonNIcon, TRUE);
    DrawExtendGraph(RESULT_ENEMY_IMAGE_LEFT + RESULT_ENEMY_IMAGE_OFFSET, RESULT_ENEMY_IMAGE_TOP, RESULT_ENEMY_IMAGE_RIGHT + RESULT_ENEMY_IMAGE_OFFSET, RESULT_ENEMY_IMAGE_BOTTOM, skeletonIcon, TRUE);

    // 今回のプレイで倒した敵の数・スコアを表示
    DrawFormatStringToHandle(RESULT_TEXT_NORMAL_X, RESULT_TEXT_KILL_Y, GetColor(255, 255, 255), enemyInfoFont, "KILLS  %d", enemyManager.GetKillCount(ENEMY_NORMAL));
    DrawFormatStringToHandle(RESULT_TEXT_WARRIOR_X, RESULT_TEXT_KILL_Y, GetColor(255, 255, 255), enemyInfoFont, "KILLS  %d", enemyManager.GetKillCount(ENEMY_WARRIOR));
    DrawFormatStringToHandle(RESULT_TEXT_NORMAL_X, RESULT_TEXT_SCORE_Y, GetColor(255, 255, 255), enemyInfoFont, "SCORE  %d", scoreManager.GetScoreByEnemyType(ENEMY_NORMAL));
    DrawFormatStringToHandle(RESULT_TEXT_WARRIOR_X, RESULT_TEXT_SCORE_Y, GetColor(255, 255, 255), enemyInfoFont, "SCORE  %d", scoreManager.GetScoreByEnemyType(ENEMY_WARRIOR));
}

void Result::DrawPlayResult(ScoreManager& scoreManager) const {

    // 総スコアを表示
    DrawFormatStringToHandle(150, 500, GetColor(255, 255, 255), textFont, "TOTAL %d", scoreManager.GetScore());

    // スコアを元にランクを表示(ランク判定を依頼)
    DrawFormatStringToHandle(520, 500, GetColor(255, 255, 255), textFont, "RANK  %s", GetRank(scoreManager.GetScore()));

    // 生存時間を表示
    DrawFormatStringToHandle(780, 120, GetColor(255, 255, 255), textFont, "TIME %02d分:%02d秒", scoreManager.GetMinutes(), scoreManager.GetSeconds());

    DrawBox(500, 570, 800, 620, GetColor(20, 20, 20), TRUE);
    DrawString(560, 590, "Rキーでタイトルに戻る", GetColor(255, 255, 255));
}

void Result::Draw(EnemyManager& enemyManager, ScoreManager& scoreManager, RankingManager& rankingManager, FontManager& fontManager) const {

    DrawPanel();

    DrawEnemyInfo(enemyManager, scoreManager);

    rankingManager.DrawRanking(fontManager);

    DrawPlayResult(scoreManager);
}

// ランク判定
const char* Result::GetRank(int score) const {

    if (score >= RANK_S_SCORE) return "S";
    if (score >= RANK_A_SCORE) return "A";
    if (score >= RANK_B_SCORE) return "B";
    if (score >= RANK_C_SCORE) return "C";
    return "D";
}