#pragma once

// ‘O•ûéŒ¾
struct ExpItemData;

class ExpItemDataLoader {
public:

	static bool Load(ExpItemData& outData);

private:

	static bool LoadData(const char* jsonPath, ExpItemData& outData);
};