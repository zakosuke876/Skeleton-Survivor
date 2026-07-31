#pragma once

// ‘O•ûéŒ¾
struct PlayerStatus;

class PlayerDataLoader {
public:

	static bool Load(PlayerStatus& outData);

private:

	static bool LoadData(const char* jsonPath, PlayerStatus& outData);
};