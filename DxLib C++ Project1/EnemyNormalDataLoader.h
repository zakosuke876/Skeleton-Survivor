#pragma once

// ‘O•ûéŒ¾
struct SkeletonNormalStatus;

class EnemyNormalDataLoader {
public:

	static bool Load(SkeletonNormalStatus& outData);

private:

	static bool LoadData(const char* jsonPath, SkeletonNormalStatus& outData);
};