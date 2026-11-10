#pragma once

class Player;
class ScoreManager;
class ItemManager;
class SoundManager;
class FontManager;
class Ground;
class PauseScene;
class Result;
class EnemyManager;
class RankingManager;
class Camera;
class InputManager;


// 各シーンの生成に必要な依存をまとめた構造体。Gameで詰めて、SceneManagerが各シーンへ渡す。
struct SceneContext {
	Player& player;
	ScoreManager& scoreManager;
	ItemManager& itemManager;
	SoundManager& soundManager;
	FontManager& fontManager;
	Ground& ground;
	PauseScene& pauseScene;
	Result& result;
	EnemyManager& enemyManager;
	RankingManager& rankingManager;
	Camera& camera;
	const InputManager& inputManager;
};
