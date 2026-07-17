#pragma once

enum class SceneType {
	Title,
	Game,
	None, // 「まだ切り替えていない」状態
};

class Scene {
public:

	virtual ~Scene() = default;

	// このシーンに入った瞬間の処理
	virtual void OnEnter() {}

	// このシーンを出る瞬間の処理
	virtual void OnExit()  {}

	virtual SceneType Update(float deltaTime) = 0;

	virtual void Draw() = 0;
};