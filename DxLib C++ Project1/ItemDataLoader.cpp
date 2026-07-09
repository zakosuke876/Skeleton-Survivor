#include "ItemDataLoader.h"
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

	// JSONファイルを開く
	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	// JSONデータを格納する変数
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

			// 回復量を取得
			data.recoveryValue = item.at("recoveryValue").get<int>();

			// 画像パスを取得
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

	// JSONファイルを開く
	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	// JSONデータを格納する変数
	json j;

	try
	{
		// JSONファイルの内容を読み込む
		file >> j;

		// "expItem"キーが存在しない場合は処理しない
		if (!j.contains("expItem")) return false;

		const json& item = j["expItem"];

		// 経験値量を取得
		outData.expValue = item.at("expValue").get<int>();

		// 画像パスを取得
		outData.graphPath = item.at("graphPath").get<std::string>();
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}

	return true;
}

bool ItemDataLoader::LoadDropConfig(const char* jsonPath, ItemDropConfig& outData) {

	// JSONファイルを開く
	std::ifstream file(jsonPath);

	// ファイルが開けなかった場合失敗
	if (!file.is_open()) return false;

	// JSONデータを格納する変数
	json j;

	try
	{
		// JSONファイルの内容を読み込む
		file >> j;

		// "ItemDropConfig"キーが存在しない場合は処理しない
		if (!j.contains("ItemDropConfig")) return false;

		const json& config = j["ItemDropConfig"];

		// アイテムドロップ率を取得する
		outData.dropRate = config.at("dropRate").get<int>();

		// ドロップ時の座標をずらすためのY方向オフセットを取得する
		outData.dropOffsetY = config.at("dropOffsetY").get<float>();

		// 経験値アイテムの生成座標をずらす範囲を取得する
		outData.expItemRandomOffset = config.at("expItemRandomOffset").get<float>();

		// アイテム種類ごとのドロップ確率を取得する
		const json& typeRate = config.at("itemTypeRate");

		// 回復アイテム(小)のドロップ率を取得する
		outData.itemTypeRate.smallRate = typeRate.at("small").get<int>();

		// 回復アイテム(中)のドロップ率を取得する
		outData.itemTypeRate.mediumRate = typeRate.at("medium").get<int>();

		// 回復アイテム(大)のドロップ率を取得する
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