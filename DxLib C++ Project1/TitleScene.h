#pragma once

// 前方宣言
class FontManager;
class RankingManager;
class Result;

// ボタン選択の管理
enum ButtonIndex{
	PLAY,
	MANUAL,
	RANKING,
	QUIT,
};

enum TitleState {
	TITLE,
	MANUAL_MENU,
	RANKING_MENU,
};

class TitleScene {
public:

	TitleScene();
	~TitleScene();
	void Reset(); // ゲームリセット処理
	void Update();
	void Draw(const FontManager& fontManager, Result& result, const RankingManager& rankingManager) const;

	bool GetisStart() const { return isStart; }
	bool GetisQuit()  const { return isQuit; }

private:

	int CalcButtonTop(int index) const;
	int CalcTextX(int left, int width, const char* text) const;
	int CalcTextY(int top, int height) const;

	void ButtonMove();
	void ButtonSelect();
	void BackToTitle();

	void TitleUI(const FontManager& fontManager)   const;
	void ManualUI()  const;
	void UpdateManual();
	void RankingUI(const FontManager& fontManager, const RankingManager& rankingManager) const;
	void UpdateRanking();

	// タイトルに表示する敵の画像ハンドル
	int skeletonNGrHandle = -1;
	int skeletonWGrHandle = -1;

	// タイトルボタン描画座標
	static constexpr int TITLE_BUTTON_START_X = 530;
	static constexpr int TITLE_BUTTON_START_Y = 300;
	static constexpr int TITLE_BUTTON_START_WIDTH = 250;
	static constexpr int TITLE_BUTTON_START_HEIGHT = 70;

	// 各ボタン同士の間隔・隙間
	static constexpr int TITLE_BUTTON_SPACING_Y = 100;

	// 配置するボタンの数
	static constexpr int TITLE_BUTTON_MAX = 4;

	// ボタンに表示する文字列
	static constexpr const char* titleButtonTexts[TITLE_BUTTON_MAX] = { "ゲーム開始", "操作説明", "ランキング", "ゲーム終了" };

	// マニュアルボタン描画座標
	static constexpr int MANUAL_BUTTON_START_X = 70;
	static constexpr int MANUAL_BUTTON_START_Y = 650;

	static constexpr int MANUAL_BUTTON_WIDTH = 180;
	static constexpr int MANUAL_BUTTON_HEIGHT = 50;
	static constexpr int MANUAL_BUTTON_TOP = 650;

	static constexpr int BUTTON_TEXT_FONT_SIZE = 16;
	
	// どのボタンを選んでいるか保持する
	int currentButtonIndex = 0;

	TitleState titleState = TITLE;

	// タイトルロゴのテキスト座標
	static constexpr int TITLE_LOGO_TEXT_X = 400;
	static constexpr int TITLE_LOGO_TEXT_Y = 50;

	// タイトル画面キャラクター(左)の座標
	static constexpr int TITLE_SKELETON_W_X = 0;
	static constexpr int TITLE_SKELETON_W_Y = 50;

	// タイトル画面キャラクター(右)の座標
	static constexpr int TITLE_SKELETON_N_X = 850;
	static constexpr int TITLE_SKELETON_N_Y = 100;

	// マニュアル画像の座標
	static constexpr int MANUAL_IMAGE_X = 0;
	static constexpr int MANUAL_IMAGE_Y = 0;

	// マニュアル画面ページ送りテキストのY座標
	static constexpr int MANUAL_NAV_TEXT_Y = 700;

	// マニュアル画面[次のページ]テキストのX座標 
	static constexpr int MANUAL_NEXT_PAGE_TEXT_X = 700;

	// マニュアル画面[前のページ]テキストのX座標
	static constexpr int MANUAL_PREV_PAGE_TEXT_X = 430;

	// マニュアル画面：「閉じる」テキストの座標
	static constexpr int MANUAL_CLOSE_TEXT_X = 580;
	static constexpr int MANUAL_CLOSE_TEXT_Y = 700;

	// ランキング画面：「閉じる」テキストの座標
	static constexpr int RANKING_CLOSE_TEXT_X = 600;
	static constexpr int RANKING_CLOSE_TEXT_Y = 680;

	// キーの入力状態チェック
	char prevKey[256]{};
	char currentKey[256]{};

	// タイトルボタンの選択管理
	int manualGrHandle = -1;

	// 操作説明のページ数
	static constexpr int MANUAL_PAGE_MAX = 2;

	// 画像を保持する配列
	int manualGrHandles[MANUAL_PAGE_MAX]{ -1, -1 };

	// 操作説明パネルのページ管理
	int manualPanelIndex = -1;

	// ゲームをスタートさせるかどうか
	bool isStart = false;

	// ゲームを終了させるかどうか
	bool isQuit = false;
};