#include "GameScene.h"
#include "DxLib.h"

GameScene::GameScene(Player& player, ScoreManager& scoreManager, ItemManager& itemManager,
                     SoundManager& soundManager, FontManager& fontManager, Ground& ground,
                     PauseScene& pauseScene, Result& result, EnemyManager& enemyManager,
                     RankingManager& rankingManager, Camera& camera)
    : player(player), scoreManager(scoreManager), itemManager(itemManager),
    soundManager(soundManager), fontManager(fontManager), ground(ground),
    pauseScene(pauseScene), result(result), enemyManager(enemyManager),
    camera(camera), rankingManager(rankingManager) {
}

void GameScene::OnEnter() {

    // ゲームシーンに入った瞬間の処理
    state = GAME_PLAYING;
	soundManager.PlayBgm(BGM_PLAY);
}

void GameScene::OnExit() {

    // ゲームシーンを出る瞬間の処理
    soundManager.StopBgm(BGM_PLAY);
}


SceneType GameScene::Update(float deltaTime) {

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


            magicEffect.Update(deltaTime);

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

                return SceneType::Title;
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

                return SceneType::Title;
            }

            break;
    }

    return SceneType::None;
}

void GameScene::Draw() {

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
        magicEffect.Draw();

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
}