#pragma once

// ‘O•ûéŒ¾
struct SkeletonWarriorStatus;

class EnemyWarriorDataLoader {
public:

	static bool Load(SkeletonWarriorStatus& outData);

private:

	static bool LoadData(const char* jsonPath, SkeletonWarriorStatus& outData);
};