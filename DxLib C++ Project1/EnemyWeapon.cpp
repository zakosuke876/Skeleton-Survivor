#include "EnemyWeapon.h"

EnemyWeapon::EnemyWeapon() {}

EnemyWeapon::~EnemyWeapon() {

	// リソース開放
	MV1DeleteModel(weaponHandle);
}

void EnemyWeapon::SetUp(int srcModelHandle, int texHandle, const char* boneNamae) {

    // 元モデルから複製
    weaponHandle = MV1DuplicateModel(srcModelHandle);

    if (texHandle != -1)
    {
        // テクスチャ読み込み
        MV1SetTextureGraphHandle(weaponHandle, 0, texHandle, FALSE);
    }
}

void EnemyWeapon::Update(int enemyModelHandle) {

    // ボーンインデックスが未取得なら検索
    if (handFrameIndex == -1)
    {
        handFrameIndex = MV1SearchFrame(enemyModelHandle, "handslot.r");
    }

    MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(enemyModelHandle, handFrameIndex);
    MATRIX offset = MGetScale(VGet(scale, scale, scale));
    MATRIX weaponMatrix = MMult(offset, handMatrix);

    MV1SetMatrix(weaponHandle, weaponMatrix);
    weaponPos = VTransform(VGet(0, tipOffset, 0), weaponMatrix);
}

void EnemyWeapon::Draw() const {

    MV1DrawModel(weaponHandle);
}