#include "TitleScene.h"
#include "GameConfig.h"
#include "FontManager.h"
#include "RankingManager.h"
#include "InputManager.h"
#include "DxLib.h"
#include <algorithm>

TitleScene::TitleScene(const TitleContext& context)
	: soundManager(context.soundManager), fontManager(context.fontManager), inputManager(context.inputManager), rankingManager(context.rankingManager) {

	// 画像読み込み
	manualGrHandles[0] = LoadGraph("Textures/ManualImage/Manual_0.png");
	manualGrHandles[1] = LoadGraph("Textures/ManualImage/Manual_1.png");
	skeletonNGrHandle = LoadGraph("UI/Enemy/Skeleton_N_Image.png");
	skeletonWGrHandle = LoadGraph("UI/Enemy/Skeleton_W_Image.png");

	// ボタン選択位置リセット
	currentButtonIndex = 0;

	// 操作説明のページリセット
	manualPanelIndex = 0;
}

TitleScene::~TitleScene() {

	// リソース開放
	for (int i = 0; i < MANUAL_PAGE_MAX; i++)
	{
		DeleteGraph(manualGrHandles[i]);
	}

	DeleteGraph(skeletonNGrHandle);
	DeleteGraph(skeletonWGrHandle);
}

void TitleScene::OnEnter() {

	//　タイトルシーンに入った瞬間の処理
	titleState = TitleState::Title;
	currentButtonIndex = 0;
	soundManager.PlayBgm(BgmType::Title);
}

void TitleScene::OnExit() {

	// タイトルシーンを出る瞬間の処理
	soundManager.StopBgm(BgmType::Title);
}

SceneType TitleScene::Update(float deltaTime) {

	switch (titleState)
	{
		case TitleState::Title:

			ButtonMove();

			return ButtonSelect();

			break;

		case TitleState::ManualMenu:

			UpdateManual();

			break;

		case TitleState::RankingMenu:

			UpdateRanking();

			break;
	}

	return SceneType::None;
}

void TitleScene::ButtonMove() {


	if (inputManager.IsPressed(KEY_INPUT_UP))
	{
		currentButtonIndex--;

		// 先頭を超えた場合
		if (currentButtonIndex < 0)
		{
			// 最後のボタンへ移動する
			currentButtonIndex = static_cast<int>(ButtonIndex::Count) - 1;
		}
	}

	if (inputManager.IsPressed(KEY_INPUT_DOWN))
	{
		currentButtonIndex++;

		// 末尾を超えた場合
		if (currentButtonIndex >= static_cast<int>(ButtonIndex::Count))
		{
			// 最初のボタンへ移動する
			currentButtonIndex = 0;
		}
	}
}

SceneType TitleScene::ButtonSelect() {

	// タイトル状態でなければボタン操作しない
	if (titleState != TitleState::Title) return SceneType::None;

	// スペースキーを押した瞬間だけ以降を処理する
	if (!inputManager.IsPressed(KEY_INPUT_SPACE)) return SceneType::None;

	// スペースを押したときの状態によって処理を分ける
	switch (static_cast<ButtonIndex>(currentButtonIndex))
	{
		case ButtonIndex::Play:

			return SceneType::Game;

			break;

		case ButtonIndex::Manual:

			titleState = TitleState::ManualMenu;

			break;

		case ButtonIndex::Ranking:

			titleState = TitleState::RankingMenu;

			break;

		case ButtonIndex::Quit:

			return SceneType::Quit;

			break;

		default:

			break;
	}

	return SceneType::None;
}


void TitleScene::BackToTitle() {

	// タイトル状態に戻す
	if (inputManager.IsPressed(KEY_INPUT_ESCAPE)) titleState = TitleState::Title;
}

void TitleScene::TitleUI(const FontManager& fontManager) const {

	int buttonLeft = TITLE_BUTTON_START_X;
	int buttonRight = TITLE_BUTTON_START_X + TITLE_BUTTON_START_WIDTH;

	// ボタンをCount個描画する
	for (int i = 0; i < static_cast<int>(ButtonIndex::Count); i++)
	{
		// 選択中のボタンの枠線は黄色、それ以外はグレーにする
		int color = (currentButtonIndex == i) ? GetColor(255, 255, 0) : GetColor(180, 180, 180);

		int currentTop = CalcButtonTop(i);
		int currentBottom = currentTop + TITLE_BUTTON_START_HEIGHT;

		// ずらしながらボタンを配置(BUTTON_MAXの数)
		DrawBox(buttonLeft, currentTop, buttonRight, currentBottom, GetColor(80, 80, 80), TRUE);
		DrawLineBox(buttonLeft, currentTop, buttonRight, currentBottom, color, TRUE); // 枠線

		// テキスト表示位置を計算
		int textX = CalcTextX(buttonLeft, TITLE_BUTTON_START_WIDTH, titleButtonTexts[i]);
		int textY = CalcTextY(CalcButtonTop(i), TITLE_BUTTON_START_HEIGHT);

		DrawString(textX, textY, titleButtonTexts[i], GetColor(255, 255, 255));
	}

	DrawStringToHandle(TITLE_LOGO_TEXT_X, TITLE_LOGO_TEXT_Y, "Skeleton Survivor", GetColor(255, 0, 0), fontManager.GetTitleFont());

	DrawGraph(TITLE_SKELETON_W_X, TITLE_SKELETON_W_Y, skeletonWGrHandle, TRUE);
	DrawGraph(TITLE_SKELETON_N_X, TITLE_SKELETON_N_Y, skeletonNGrHandle, TRUE);
}

void TitleScene::ManualUI() const {

	// 操作説明用画像を表示
	DrawGraph(MANUAL_IMAGE_X, MANUAL_IMAGE_Y, manualGrHandles[manualPanelIndex], TRUE);

	switch (manualPanelIndex)
	{
	case 0:

		DrawString(MANUAL_NEXT_PAGE_TEXT_X, MANUAL_NAV_TEXT_Y, "次のページ:→", GetColor(255, 0, 0));

		break;

	case 1:

		DrawString(MANUAL_PREV_PAGE_TEXT_X, MANUAL_NAV_TEXT_Y, "前のページ:←", GetColor(255, 0, 0));

		break;

	default:

		break;
	}

	DrawString(MANUAL_CLOSE_TEXT_X, MANUAL_CLOSE_TEXT_Y, "閉じる:Esc", GetColor(255, 0, 0));
}

void TitleScene::UpdateManual() {

	// Manual状態でなければ処理しない
	if (titleState != TitleState::ManualMenu) return;

	// キーを押した瞬間だけ処理する
	if (inputManager.IsPressed(KEY_INPUT_LEFT))
	{
		// 1ページ戻す
		manualPanelIndex--;

		// 範囲制限
		manualPanelIndex = std::clamp(manualPanelIndex, 0, MANUAL_PAGE_MAX - 1);
	}

	if (inputManager.IsPressed(KEY_INPUT_RIGHT))
	{
		manualPanelIndex++;

		manualPanelIndex = std::clamp(manualPanelIndex, 0, MANUAL_PAGE_MAX - 1);
	}

	BackToTitle();
}

void TitleScene::RankingUI(const FontManager& fontManager, const RankingManager& rankingManager) const {

	// Ranking状態でなければ処理しない
	if (titleState != TitleState::RankingMenu) return;

	rankingManager.DrawTitleRanking(fontManager);

	DrawString(RANKING_CLOSE_TEXT_X, RANKING_CLOSE_TEXT_Y, "閉じる:Esc", GetColor(255, 0, 0));
}

void TitleScene::UpdateRanking() {

	BackToTitle();
}

void TitleScene::Draw() {

	if (titleState == TitleState::Title)
	{
		TitleUI(fontManager);
	}

	if (titleState == TitleState::ManualMenu)
	{
		ManualUI();
	}

	if (titleState == TitleState::RankingMenu)
	{
		RankingUI(fontManager, rankingManager);
	}
}

int TitleScene::CalcButtonTop(int index) const {

	return TITLE_BUTTON_START_Y + TITLE_BUTTON_SPACING_Y * index;
}

int TitleScene::CalcTextX(int left, int width, const char* text) const {

	int textWidth = GetDrawStringWidth(text, strlen(text));
	return left + (width - textWidth) / 2;
}

int TitleScene::CalcTextY(int top, int height) const {

	return top + (height - BUTTON_TEXT_FONT_SIZE) / 2;
}
