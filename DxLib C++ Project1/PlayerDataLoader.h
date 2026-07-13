#pragma once
#include "PlayerStatus.h"

class PlayerDataLoader {
public:

	static bool Load(PlayerStatus& outData);



	//bool Load();

	//PlayerStatus GetLoadData() { return playerDataTable; }

private:

	static bool LoadData(const char* jsonPath, PlayerStatus& outData);

	//bool LoadData(const char* jsonPath);

	// JSONデータ保存用構造体
	//PlayerStatus playerDataTable;
};