#pragma once
#include "Scene.h"
#include "SceneContext.h"
#include <memory>

class SceneManager {
public:

	SceneManager(const SceneContext& context);
	void Update(float deltaTime);
	void Draw() const;

	bool IsGameFinished() const { return isGameFinished; }

private:

	SceneContext context;

	// 受け取ったシーンを生成する
	std::unique_ptr<Scene> CreateScene(SceneType type);

	void ChangeScene(SceneType nextScene);

	// 最初はタイトルから
	SceneType currentScene = SceneType::Title;

	// 現在のシーンを所有
	std::unique_ptr<Scene> activeScene;

	// ゲームを終了させるかどうか
	bool isGameFinished = false;
};
