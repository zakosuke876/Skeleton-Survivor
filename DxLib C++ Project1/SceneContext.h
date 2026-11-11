#pragma once

// 前方宣言
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
	const SoundManager& soundManager;
	const FontManager& fontManager;
	const Ground& ground;
	const Result& result;
	const InputManager& inputManager;
	Player& player;
	ScoreManager& scoreManager;
	ItemManager& itemManager;
	PauseScene& pauseScene;
	EnemyManager& enemyManager;
	RankingManager& rankingManager;
	Camera& camera;
};
