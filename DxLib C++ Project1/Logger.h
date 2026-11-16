#pragma once
#include <string>

enum class LogLevel {
	Info,  // 動作確認・情報
	Warn,  // 警告
	Error, // 失敗・対処必須
};

void WriteLog(LogLevel level, const std::string& message);

