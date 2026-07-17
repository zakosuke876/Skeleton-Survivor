#pragma once
#include "Scene.h"
#include "TitleScene.h"
#include "FontManager.h"
#include "RankingManager.h"
#include "Result.h"
#include "SoundManager.h"

class TitleSceneWrapper : public Scene {
public:

	TitleSceneWrapper(FontManager& fontManager, RankingManager& rankingManager, Result& result, SoundManager& soundManager)
		: fontManager(fontManager), rankingManager(rankingManager), result(result), soundManager(soundManager){}

    void OnEnter() override {
        titleScene.Reset();
        soundManager.PlayBgm(BGM_TITLE);
    }

    void OnExit() override {
        soundManager.StopBgm(BGM_TITLE);
    }

    SceneType Update(float /*deltaTime*/) override {
        titleScene.Update();

        // ŠJn‚ª‰Ÿ‚³‚ê‚½‚çGame‚Ö‘JˆÚ
        if (titleScene.GetisStart()) return SceneType::Game;

        // Ø‚è‘Ö‚¦‚È‚¢
        return SceneType::None;
    }

    void Draw() override {
        titleScene.Draw(fontManager, result, rankingManager);
    }

    bool WantsQuit() const { return titleScene.GetisQuit(); }

private:

    TitleScene titleScene;
    FontManager& fontManager;
    RankingManager& rankingManager;
    Result& result;
    SoundManager& soundManager;
};