#include <Windows.h>
#include "Logger.h"

// このファイルだけで使う
namespace {

	// レベルに応じたタグを返す
	const char* GetLogLevelTag(LogLevel level) {

		switch (level)
		{
			case LogLevel::Info:  return "[Info] ";
			case LogLevel::Warn:  return "[Warn] ";
			case LogLevel::Error: return "[Error] ";
			default:              return "[Warn] ";
		}
	}
}


void WriteLog(LogLevel level, const std::string& message) {

#ifdef _DEBUG

	OutputDebugStringA((GetLogLevelTag(level) + message + "\n").c_str());

#endif

}
