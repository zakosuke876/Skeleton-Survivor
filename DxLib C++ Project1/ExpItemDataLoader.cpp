#include "ExpItemDataLoader.h"
#include "ExpItem.h"
#include "JsonPath.h"
#include "JsonLoader.h"

#include "include/json.hpp"
using json = nlohmann::json;

bool ExpItemDataLoader::Load(ExpItemData& outData) {

	// JsonPathÅEç\ë¢ëÃÇìnÇ∑
	return LoadData(JsonPath::EXP_ITEM_DATA, outData);
}

bool ExpItemDataLoader::LoadData(const char* jsonPath, ExpItemData& outData) {

	return JsonLoader::Load(jsonPath, "expItem", [&](const json& data) {

		outData.expValue = data.at("expValue").get<int>();

		outData.graphPath = data.at("graphPath").get<std::string>();

		return true;
	});
}