#include "RecoveryItemDataLoader.h"
#include "JsonPath.h"
#include <fstream>
#include "DxLib.h"

#include "include/json.hpp"
using json = nlohmann::json;

RecoveryItemType RecoveryItemDataLoader::StringToRecoveryType(const std::string& str) {

	if (str == "RECOVERY_SMALL") return RecoveryItemType::RECOVERY_SMALL;
	if (str == "RECOVERY_MEDIUM") return RecoveryItemType::RECOVERY_MEDIUM;
	if (str == "RECOVERY_LARGE") return RecoveryItemType::RECOVERY_LARGE;

	// 想定外の文字列の場合
	return RecoveryItemType::ITEM_TYPE_MAX;
}

bool RecoveryItemDataLoader::Load(std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable) {

	return LoadData(JsonPath::RECOVERY_ITEM_DATA, outTable);
}

bool RecoveryItemDataLoader::LoadData(const char* jsonPath, std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable) {

	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open())
	{
		printfDx("ファイルが開けません: %s\n", jsonPath);
		return false;
	}

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