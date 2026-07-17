#include "Game.h"
#include "GameConfig.h"

Game::Game() {

    SetMainWindowText("Skeleton Survivor");
    SetDrawScreen(DX_SCREEN_BACK);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    srand((unsigned int)time(nullptr));

    // 前フレームの時刻を記録
    prevTime = GetNowCount();

    // 起動時にタイトルBGMを再生
    soundManager.PlayBgm(BGM_TITLE);

    // 起動時にタイトルシーンに設定
    currentScene = APP_TITLE;

    titleWrapper = std::make_unique<TitleSceneWrapper>(fontManager, rankingManager, result, soundManager);

    gameScene = std::make_unique<GameScene>(player, scoreManager, itemManager, soundManager, fontManager, ground,
        pauseScene, result, enemyManager, rankingManager, camera);
}

void Game::Run() {

    while (ProcessMessage() == 0 && !titleWrapper->WantsQuit())
    {
        // 現在時刻取得()
        int nowTime = GetNowCount();

        // 前フレームとの差を秒に変換
        deltaTime = (nowTime - prevTime) / 1000.0f;

        //現在時刻を保存
        prevTime = nowTime;

        Update();
        Draw();
    }
}

void Game::Update() {

    switch (currentScene)
    {
        case APP_TITLE: {

            SceneType next = titleWrapper->Update(deltaTime);

            if (next == SceneType::Game)
            {
                titleWrapper->OnExit();
                GameReset();
                gameScene->OnEnter();
                currentScene = APP_GAME;
            }

            break;
        }

        case APP_GAME: {

            SceneType next = gameScene->Update(deltaTime);

            if (next == SceneType::Title)
            {
                gameScene->OnExit();
                titleWrapper->OnEnter();
                currentScene = APP_TITLE;
            }

            break;
        }


        default:

            break;
    }
}

void Game::Draw() {

    ClearDrawScreen();

    switch (currentScene)
    {
        case APP_TITLE:

            titleWrapper->Draw();

            break;

        case APP_GAME:

            gameScene->Draw();

            break;

        default:

            break;
    }

    ScreenFlip();
}

void Game::GameReset() {

    // ゲームリセット処理
    player.Reset();
    enemyManager.Reset();
    itemManager.Reset();
    scoreManager.Reset();
    rankingManager.Reset();
    titleScene.Reset();
    pauseScene.Reset();
}