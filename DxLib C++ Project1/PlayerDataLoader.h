#pragma once

// 前方宣言
struct PlayerStatus;

class PlayerDataLoader {
public:

	static bool Load(PlayerStatus& outData);

private:

	static bool LoadData(const char* jsonPath, PlayerStatus& outData);
};