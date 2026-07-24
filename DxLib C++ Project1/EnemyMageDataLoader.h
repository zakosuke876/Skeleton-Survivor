#pragma once


struct SkeletonMageStatus;

class EnemyMageDataLoader {
public:

	static bool Load(SkeletonMageStatus& outData);

private:

	static bool LoadData(const char* jsonPath, SkeletonMageStatus& outData);

};