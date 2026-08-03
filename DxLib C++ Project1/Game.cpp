#include "Game.h"
#include "GameConfig.h"

Game::Game() {

    // ウィンドウ・3D描画まわりの初期設定
    SetMainWindowText("Skeleton Survivor");
    SetDrawScreen(DX_SCREEN_BACK);
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // 乱数の種を初期化
    srand((unsigned int)time(nullptr));

    // 前フレームの時刻を記録
    prevTime = GetNowCount();

    // 起動時にタイトルBGMを再生
    soundManager.PlayBgm(BGM_TITLE);

    // 起動時はタイトルシーンから開始
    currentScene = APP_TITLE;

    // 各シーンを生成（必要な参照を渡す）
    titleScene = std::make_unique<TitleScene>(fontManager, rankingManager, soundManager, result);

    gameScene = std::make_unique<GameScene>(player, scoreManager, itemManager, soundManager, fontManager, ground,
        pauseScene, result, enemyManager, rankingManager, camera);
}

void Game::Run() {

    // メインループ：ウィンドウが閉じられるか終了要求があるまで回す
    while (ProcessMessage() == 0 && !titleScene->GetisQuit())
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

    // 現在のシーンごとに更新処理を振り分ける
    switch (currentScene)
    {
        // タイトルシーンの更新
        case APP_TITLE: {

            SceneType next = titleScene->Update(deltaTime);

            // ゲーム開始が選ばれたらゲームシーンへ遷移
            if (next == SceneType::Game)
            {
                titleScene->OnExit();
                GameReset();          // ゲーム状態をリセット
                gameScene->OnEnter();
                currentScene = APP_GAME;
            }

            break;
        }

        // ゲームシーンの更新
        case APP_GAME: {

            SceneType next = gameScene->Update(deltaTime);

            // タイトルへ戻る要求があればタイトルシーンへ遷移
            if (next == SceneType::Title)
            {
                gameScene->OnExit();
                titleScene->OnEnter();
                currentScene = APP_TITLE;
            }

            break;
        }


        default:

            break;
    }
}

void Game::Draw() {

    // 描画前に画面をクリア
    ClearDrawScreen();

    // 現在のシーンごとに描画を振り分ける
    switch (currentScene)
    {
        // タイトルシーンの描画
        case APP_TITLE:

            titleScene->Draw();

            break;

        // ゲームシーンの描画
        case APP_GAME:

            gameScene->Draw();

            break;

        default:

            break;
    }

    // 裏画面を表画面へ反映
    ScreenFlip();
}

void Game::GameReset() {

    // ゲーム開始時に各オブジェクトを初期状態へ戻す
    player.Reset();
    enemyManager.Reset();
    itemManager.Reset();
    scoreManager.Reset();
    rankingManager.Reset();
    titleScene->Reset();
    pauseScene.Reset();
}
