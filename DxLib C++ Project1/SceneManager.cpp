#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

SceneManager::SceneManager(const SceneContext& context)
	: context(context)
{
	// 初期シーン(タイトル)を生成
	ChangeScene(SceneType::Title);
}

void SceneManager::Update(float deltaTime) {

	SceneType next = activeScene->Update(deltaTime);

	if (next == SceneType::None) return;

	// ゲーム終了要求が来た場合は終了フラグをtrueに
	if (next == SceneType::Quit)
	{
		isGameFinished = true;
		return;
	}

	// 違うシーンへの遷移要求を受け取った場合
	if (next != currentScene) ChangeScene(next);
}

void SceneManager::Draw() const {

	activeScene->Draw();
}

void SceneManager::ChangeScene(SceneType nextScene) {

	// 初回は前シーンが無いため呼ばない
	if (activeScene) activeScene->OnExit();

	// 他のシーンへ遷移
	currentScene = nextScene;

	// 次のシーンの実体を生成して差し替える
	activeScene = CreateScene(nextScene);
	
	if (activeScene) activeScene->OnEnter();
}

std::unique_ptr<Scene> SceneManager::CreateScene(SceneType scene) {

	switch (scene)
	{
		case SceneType::Title:
		{
			TitleScene::TitleContext titleContext { 
				  context.soundManager, context.fontManager,
				  context.inputManager, context.rankingManager
				};

			return std::make_unique<TitleScene>(titleContext);
		}

		case SceneType::Game:
		{
			return std::make_unique<GameScene>(context);
		}

		default:
		{
			return nullptr;
		}
	}
}
