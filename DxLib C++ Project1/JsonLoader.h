#pragma once
#include "include/json.hpp"
#include <functional>

namespace JsonLoader {

	// pathを開いてパースし、j[key]を取り出してfill(j[key])を実行する
	bool Load(const char* path, const char* key, const std::function<bool(const nlohmann::json&)>& fill);
}
