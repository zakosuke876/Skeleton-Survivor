#pragma once
#include "Player.h"
#include <string>

class PlayerDataLoader {
public:

	// ¬Œ÷‚µ‚½‚çtrue‚ğ•Ô‚µAoutData‚ÉŒ‹‰Ê‚ğŠi”[‚·‚é
	static bool LoadPlayerData(const char* jsonPath, PlayerStatus& outData);

private:
};