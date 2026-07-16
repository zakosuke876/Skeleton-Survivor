#include "ItemDataLoader.h"
#include "JsonPath.h"
#include <fstream>

#include "include/json.hpp"
using json = nlohmann::json;

RecoveryItemType ItemDataLoader::StringToRecoveryType(const std::string& str) {

	if (str == "RECOVERY_SMALL") return RecoveryItemType::RECOVERY_SMALL;
	if (str == "RECOVERY_MEDIUM") return RecoveryItemType::RECOVERY_MEDIUM;
	if (str == "RECOVERY_LARGE") return RecoveryItemType::RECOVERY_LARGE;

	// 想定外の文字列の場合
	return RecoveryItemType::ITEM_TYPE_MAX;
}

bool ItemDataLoader::LoadRecoveryItemData(const char* jsonPath, std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable) {

	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	json j;

	try
	{
		// JSONファイルの内容を読み込む
		file >> j;

		// "recoveryItems"キーが存在しない場合は処理しない
		if (!j.contains("recoveryItems")) return false;

		// recoveryItems配列を1件ずつ取り出す
		for (const json& item : j["recoveryItems"])
		{
			// JSONからtype文字列を取得(enumへ変換)
			RecoveryItemType type = StringToRecoveryType(item.at("type").get<std::string>());

			// 不正なtype文字列はスキップ
			if (type == RecoveryItemType::ITEM_TYPE_MAX) continue;

			// RecoveryItemDataへデータをまとめる
			RecoveryItemData data;

			data.recoveryValue = item.at("recoveryValue").get<int>();

			data.graphPath = item.at("graphPath").get<std::string>();

			// enum番号を使って配列へ保存
			outTable[ToInt(type)] = data;
		}
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}

bool ItemDataLoader::LoadExpItemData(const char* jsonPath, ExpItemData& outData) {

	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	json j;

	try
	{
		file >> j;

		// "expItem"キーが存在しない場合は処理しない
		if (!j.contains("expItem")) return false;

		const json& item = j["expItem"];

		outData.expValue = item.at("expValue").get<int>();

		outData.graphPath = item.at("graphPath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}

bool ItemDataLoader::Load(ItemDropConfig& outData) {

	return LoadDropConfig(JsonPath::ITEM_DROP_CONFIG, outData);
}

bool ItemDataLoader::LoadDropConfig(const char* jsonPath, ItemDropConfig& outData) {

	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	json j;

	try
	{
		file >> j;

		// "ItemDropConfig"キーが存在しない場合は処理しない
		if (!j.contains("ItemDropConfig")) return false;

		const json& config = j["ItemDropConfig"];

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
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}