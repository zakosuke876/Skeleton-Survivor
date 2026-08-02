#pragma once

struct ItemDropConfig;

class ItemDropConfigLoader {
public:

	static bool Load(ItemDropConfig& outData);

private:

	static bool LoadData(const char* jsonPath, ItemDropConfig& outData);
};