#include "Game.h"
#include "GameConfig.h"

Game::Game()
    : ground(random), player(random, inputManager), enemyManager(random), itemManager(random), pauseScene(inputManager),
      context{soundManager, fontManager, ground, result, inputManager, player, scoreManager, itemManager, pauseScene,
              enemyManager, rankingManager, camera},
      sceneManager(context)
{

    // ウィンドウ・3D描画まわりの初期設定
    SetMainWindowText("Skeleton Survivor");
    SetDrawScreen(DX_SCREEN_BACK);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // 前フレームの時刻を記録
    prevTime = GetNowCount();

    // 起動時にタイトルBGMを再生
    soundManager.PlayBgm(BGM_TITLE);
}

void Game::Run() {

    // メインループ：ウィンドウが閉じられるか終了要求があるまで回す
    while (ProcessMessage() == 0 && !sceneManager.IsGameFinished())
    {
        // 現在時刻を取得
        int nowTime = GetNowCount();

        // 前フレームとの差を秒に変換（deltaTime）
        deltaTime = (nowTime - prevTime) / 1000.0f;

        // 現在時刻を次フレーム用に保存
        prevTime = nowTime;

        Update();
        Draw();
    }
}

void Game::Update() {

    inputManager.Update();

    sceneManager.Update(deltaTime);
}

void Game::Draw() {

    // 描画前に画面をクリア
    ClearDrawScreen();

    sceneManager.Draw();

    // 裏画面を表画面へ反映
    ScreenFlip();
}
