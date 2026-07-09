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
    currentScene = SCENE_TITLE;
}

void Game::Run() {

    while (ProcessMessage() == 0 && !titleScene.GetisQuit())
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
        case SCENE_TITLE:

            titleScene.Update();

            // タイトル画面でゲーム開始が選択された場合
            if (titleScene.GetisStart())
            {
                // ゲームを初期化
                GameReset();

                // タイトルBGMを停止
                soundManager.StopBgm(BGM_TITLE);

                // プレイBGMを再生
                soundManager.PlayBgm(BGM_PLAY);

                // ゲームシーンに遷移
                currentScene = SCENE_GAME;

                // プレイ状態へ遷移
                state = GAME_PLAYING;
            }

            break;

        case SCENE_GAME:

            switch (state)
            {
                // ゲームプレイ中
                case GAME_PLAYING:

                    pauseScene.Update();
                    
                    // ポーズボタンが押された場合
                    if (pauseScene.GetIsPause())
                    {
                        // ポーズ状態へ遷移
                        state = GAME_PAUSE;
                        break;
                    }

                    player.Update(deltaTime);
                    enemyManager.Update(player, deltaTime);
                    scoreManager.Update(deltaTime);
                    itemManager.Update();
                    camera.UpdateCamera(player.GetPosition());

                    // 当たり判定チェック
                    enemyManager.CheckPlayerAttackHit(player, scoreManager, itemManager, soundManager);
                    enemyManager.CheckEnemyAttackHit(player, soundManager);
                    itemManager.CheckPlayerCollision(player, soundManager);


                    // プレイヤーが消えた場合
                    if (!player.IsActive())
                    {
                        // スコアをランキングに追加
                        rankingManager.AddScore(scoreManager);

                        soundManager.StopBgm(BGM_PLAY);

                        soundManager.PlayBgm(BGM_GAMEOVER);

                        // ゲームオーバー状態へ遷移
                        state = GAME_OVER;
                    }

                    break;

                    // ポーズ中
                case GAME_PAUSE:

                    pauseScene.Update();

                    // ポーズボタンが押された場合
                    if (!pauseScene.GetIsPause())
                    {
                        // ゲームプレイ状態へ遷移
                        state = GAME_PLAYING;
                    }

                    // Rキーが押された場合
                    if (CheckHitKey(KEY_INPUT_R))
                    {
                        soundManager.StopBgm(BGM_PLAY);

                        soundManager.PlayBgm(BGM_TITLE);

                        currentScene = SCENE_TITLE;
                    }

                    break;

                    // ゲームオーバー
                case GAME_OVER:

                    player.Update(deltaTime);
                    enemyManager.Update(player, deltaTime);
                    camera.UpdateCamera(player.GetPosition());
                    rankingManager.Update(deltaTime);

                    // Rキーが押された場合タイトル画面に戻る
                    if (CheckHitKey(KEY_INPUT_R))
                    {
                        soundManager.StopBgm(BGM_GAMEOVER);

                        soundManager.PlayBgm(BGM_TITLE);

                        currentScene = SCENE_TITLE;
                    }

                    break;
            }

            break;

        default:

            break;
    }
}

void Game::Draw() {

    ClearDrawScreen();

    switch (currentScene)
    {
        case SCENE_TITLE:

            titleScene.Draw(fontManager, result, rankingManager);

            break;

        case SCENE_GAME:

            switch (state)
            {
                case GAME_PLAYING:

                    // Zバッファ切り替え
                    SetUseZBuffer3D(TRUE);
                    SetWriteZBuffer3D(TRUE);

                    // 3D描画 (Zバッファ有効)
                    ground.DrawGround();
                    player.Draw();
                    enemyManager.Draw();
                    itemManager.Draw();

                    // Zバッファ切り替え
                    SetUseZBuffer3D(FALSE);
                    SetWriteZBuffer3D(FALSE);

                    // 2D描画 (Zバッファ無効)
                    enemyManager.DrawUI();
                    scoreManager.Draw(fontManager);
                    player.DrawUI(fontManager);
                    pauseScene.Draw(fontManager);

                    break;

                case GAME_PAUSE:

                    // Zバッファ切り替え
                    SetUseZBuffer3D(TRUE);
                    SetWriteZBuffer3D(TRUE);

                    // 3D描画 (Zバッファ有効)
                    ground.DrawGround();
                    player.Draw();
                    enemyManager.Draw();
                    itemManager.Draw();

                    // Zバッファ切り替え
                    SetUseZBuffer3D(FALSE);
                    SetWriteZBuffer3D(FALSE);

                    // 2D描画 (Zバッファ無効)
                    enemyManager.DrawUI();
                    scoreManager.Draw(fontManager);
                    player.DrawUI(fontManager);

                    pauseScene.Draw(fontManager);

                    break;

                case GAME_OVER:

                    // Zバッファ切り替え
                    SetUseZBuffer3D(TRUE);
                    SetWriteZBuffer3D(TRUE);

                    // 3D描画 (Zバッファ有効)
                    ground.DrawGround();
                    player.Draw();
                    enemyManager.Draw();
                    itemManager.Draw();

                    // Zバッファ切り替え
                    SetUseZBuffer3D(FALSE);
                    SetWriteZBuffer3D(FALSE);

                    // 2D (Zバッファ無効)
                    enemyManager.DrawUI();
                    scoreManager.Draw(fontManager);
                    player.DrawUI(fontManager);

                    // リザルトを表示
                    result.Draw(enemyManager, scoreManager, rankingManager, fontManager);

                    break;
            }

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