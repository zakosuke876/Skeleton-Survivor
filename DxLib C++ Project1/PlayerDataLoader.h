#pragma once
#include "PlayerStatus.h"

class PlayerDataLoader {
public:

	bool Load(PlayerStatus& outData);



	//bool Load();

	//PlayerStatus GetLoadData() { return playerDataTable; }

private:

	bool LoadData(const char* jsonPath, PlayerStatus& outData);

	//bool LoadData(const char* jsonPath);

	// JSONデータ保存用構造体
	//PlayerStatus playerDataTable;
};