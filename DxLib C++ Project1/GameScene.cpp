#include "GameScene.h"
#include "Player.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "Ground.h"
#include "PauseScene.h"
#include "Result.h"
#include "EnemyManager.h"
#include "RankingManager.h"
#include "Camera.h"
#include "InputManager.h"
#include "MagicEffectManager.h"
#include "DxLib.h"

GameScene::GameScene(const SceneContext& context) 
    : soundManager(context.soundManager), fontManager(context.fontManager), ground(context.ground),
      result(context.result), inputManager(context.inputManager), player(context.player),
      scoreManager(context.scoreManager), itemManager(context.itemManager), pauseScene(context.pauseScene),
      enemyManager(context.enemyManager), rankingManager(context.rankingManager), camera(context.camera),
      magicEffectManager(context.magicEffectManager)
{

}

void GameScene::OnEnter() {

    // ゲームシーンに入った瞬間の処理
    state = GAME_PLAYING;
	soundManager.PlayBgm(BGM_PLAY);
    magicEffectManager.Reset();

    // ゲーム開始時に各オブジェクトを初期状態へ戻す
    player.Reset();
    enemyManager.Reset();
    itemManager.Reset();
    scoreManager.Reset();
    rankingManager.Reset();
    pauseScene.Reset();
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

            player.Update(deltaTime, ground);


            enemyManager.Update(player, deltaTime);
            scoreManager.Update(deltaTime);
            itemManager.Update();
            camera.UpdateCamera(player.GetPosition());
            magicEffectManager.Update(player, deltaTime);

            // 当たり判定チェック
            enemyManager.CheckPlayerAttackHit(player, scoreManager, itemManager, soundManager);
            enemyManager.CheckEnemyAttackHit(player, soundManager);
            magicEffectManager.CheckPlayerHit(player, soundManager);
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
            if (inputManager.IsPressed(KEY_INPUT_R))
            {
                soundManager.StopBgm(BGM_PLAY);

                soundManager.PlayBgm(BGM_TITLE);

                return SceneType::Title;
            }

            break;

            // ゲームオーバー
        case GAME_OVER:

            player.Update(deltaTime, ground);


            enemyManager.Update(player, deltaTime);
            camera.UpdateCamera(player.GetPosition());
            rankingManager.Update(deltaTime);

            // Rキーが押された場合タイトル画面に戻る
            if (inputManager.IsPressed(KEY_INPUT_R))
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
        //magicEffect.Draw();


        magicEffectManager.Draw();

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
