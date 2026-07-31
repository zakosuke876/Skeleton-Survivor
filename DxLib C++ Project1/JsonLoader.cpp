#include "JsonLoader.h"
#include <fstream>
#include "DxLib.h"

using json = nlohmann::json;

bool JsonLoader::Load(const char* path, const char* key, const std::function<bool(const json&)>& fill) {

	std::ifstream file(path);

	if (!file.is_open())
	{
		printfDx("ファイルが開けません: %s\n", path);
		return false;
	}

	try
	{
		json j;

		file >> j;

		if (!j.contains(key)) return false;

		// 各ローダー固有の代入だけ差し込む
		return fill(j[key]);
	}
	catch (const json::exception& e)
	{
		printfDx("JSON読み込みエラー: %s\n", e.what());
		return false;
	}
}