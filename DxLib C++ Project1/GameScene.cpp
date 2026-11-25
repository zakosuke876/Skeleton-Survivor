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
#include "EffectManager.h"
#include "DxLib.h"

GameScene::GameScene(const SceneContext& context) 
    : soundManager(context.soundManager), fontManager(context.fontManager), ground(context.ground),
      result(context.result), inputManager(context.inputManager), player(context.player),
      scoreManager(context.scoreManager), itemManager(context.itemManager), pauseScene(context.pauseScene),
      enemyManager(context.enemyManager), rankingManager(context.rankingManager), camera(context.camera),
      magicEffectManager(context.magicEffectManager), effectManager(context.effectManager)
{
    bgHandle = LoadGraph("Textures/BackGround/BackGround.png");
    moonHandle = LoadGraph("Textures/BackGround/Moon.png");
}

GameScene::~GameScene() {

    DeleteGraph(bgHandle);
    DeleteGraph(moonHandle);
}

void GameScene::OnEnter() {

    // ゲームシーンに入った瞬間の処理
    state = GameState::Playing;
	soundManager.PlayBgm(BgmType::Play);
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
    soundManager.StopBgm(BgmType::Play);
}


SceneType GameScene::Update(float deltaTime) {

    switch (state)
    {
        // ゲームプレイ中
        case GameState::Playing:

            pauseScene.Update();

            // ポーズボタンが押された場合
            if (pauseScene.GetIsPause())
            {
                // ポーズ状態へ遷移
                state = GameState::Pause;
                break;
            }

            player.Update(deltaTime, ground);
            enemyManager.Update(player, deltaTime);
            scoreManager.Update(deltaTime);
            itemManager.Update();
            camera.UpdateCamera(player.GetPosition());
            magicEffectManager.Update(player, deltaTime);
            effectManager.Update(deltaTime);

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

                soundManager.StopBgm(BgmType::Play);

                soundManager.PlayBgm(BgmType::GameOver);

                // ゲームオーバー状態へ遷移
                state = GameState::GameOver;
            }

            break;

            // ポーズ中
        case GameState::Pause:

            pauseScene.Update();

            // ポーズボタンが押された場合
            if (!pauseScene.GetIsPause())
            {
                // ゲームプレイ状態へ遷移
                state = GameState::Playing;
            }

            // Rキーが押された場合
            if (inputManager.IsPressed(KEY_INPUT_R))
            {
                soundManager.StopBgm(BgmType::Play);

                soundManager.PlayBgm(BgmType::Title);

                return SceneType::Title;
            }

            break;

            // ゲームオーバー
        case GameState::GameOver:

            player.Update(deltaTime, ground);


            enemyManager.Update(player, deltaTime);
            camera.UpdateCamera(player.GetPosition());
            rankingManager.Update(deltaTime);

            // Rキーが押された場合タイトル画面に戻る
            if (inputManager.IsPressed(KEY_INPUT_R))
            {
                soundManager.StopBgm(BgmType::GameOver);

                soundManager.PlayBgm(BgmType::Title);

                return SceneType::Title;
            }

            break;
    }

    return SceneType::None;
}

void GameScene::Draw() {

    // Zバッファ切り替え
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // 背景描画
    DrawExtendGraph(0, 0, GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT, bgHandle, TRUE);

    // 月描画
    DrawExtendGraph(MOON_X, MOON_Y, MOON_X + MOON_SIZE, MOON_Y + MOON_SIZE, moonHandle, TRUE);

    switch (state)
    {
    case GameState::Playing:

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
        effectManager.Draw();
        pauseScene.Draw(fontManager);

        break;

    case GameState::Pause:

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
        effectManager.Draw();
        pauseScene.Draw(fontManager);

        break;

    case GameState::GameOver:

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
