#pragma once

// 前方宣言
struct SkeletonMageStatus;

class EnemyMageDataLoader {
public:

	// JSONファイルからデータ読み込みを依頼
	static bool Load(SkeletonMageStatus& outData);

private:

	//　読み込んだデータを構造体へ保存する
	static bool LoadData(const char* jsonPath, SkeletonMageStatus& outData);

};