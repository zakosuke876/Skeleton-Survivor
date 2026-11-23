#include "SpriteManager.h"
#include "Logger.h"
#include "DxLib.h"

// テーブルの各シートを読み込みハンドルを格納する
SpriteManager::SpriteManager() {

	for (auto& data : animations)
	{
		data.handles.resize(data.frameCount);

		if (LoadDivGraph(data.path, data.frameCount, data.splitX, data.splitY, data.width, data.height, data.handles.data()) == -1)
		{
			WriteLog(LogLevel::Error, std::string("スプライトシートの読み込み失敗: ") + data.path);
		}
	}
}

// 保持するハンドルを解放する
SpriteManager::~SpriteManager() {

	for (auto& data : animations)
	{
		for (auto& handle : data.handles)
		{
			if (handle != -1) DeleteGraph(handle);
		}
	}
}

// 指定したエフェクトのデータを返す
AnimationClip SpriteManager::GetClip(EffectType type) const {
	
	const AnimationData& animData = animations[static_cast<int>(type)];
	return { animData.handles.data(), animData.frameCount, animData.frameTime };
}
