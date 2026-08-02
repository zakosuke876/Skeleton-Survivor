#include "ItemDropConfigLoader.h"
#include "ItemDropConfig.h"
#include "DxLib.h"
#include "JsonPath.h"
#include "JsonLoader.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool ItemDropConfigLoader::Load(ItemDropConfig& outData) {

	// JsonPath・構造体を渡す
	return LoadData(JsonPath::ITEM_DROP_CONFIG, outData);
}

bool ItemDropConfigLoader::LoadData(const char* jsonPath, ItemDropConfig& outData) {

	return JsonLoader::Load(jsonPath, "ItemDropConfig", [&](const json& config) {

		// 読み込んだ値を構造体へ設定
		outData.dropRate = config.at("dropRate").get<int>();

		outData.dropOffsetY = config.at("dropOffsetY").get<float>();

		outData.expItemRandomOffset = config.at("expItemRandomOffset").get<float>();

		const json& typeRate = config.at("itemTypeRate");

		outData.itemTypeRate.smallRate = typeRate.at("small").get<int>();

		outData.itemTypeRate.mediumRate = typeRate.at("medium").get<int>();

		outData.itemTypeRate.largeRate = typeRate.at("large").get<int>();

		// アイテムのドロップ率が合計100になっているか計算
		int total = outData.itemTypeRate.smallRate + outData.itemTypeRate.mediumRate + outData.itemTypeRate.largeRate;
		if (total != 100)
		{
			printfDx("itemTypeRateの合計が100%%になっていません(合計:%d)\n", total);
			return false;
		}

		return true;
	});
}