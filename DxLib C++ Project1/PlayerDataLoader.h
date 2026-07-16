#pragma once
#include "PlayerStatus.h"

class PlayerDataLoader {
public:

	static bool Load(PlayerStatus& outData);

private:

	// JSONファイルのパス
	static bool LoadData(const char* jsonPath, PlayerStatus& outData);

};