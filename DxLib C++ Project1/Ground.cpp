#include "Ground.h"
#include "DxLib.h"

Ground::Ground() {

    // 地面モデル読み込み
    groundHandle = MV1LoadModel("Stage/Ground.mv1");

    // 地面モデルのスケールを設定
    MV1SetScale(groundHandle, VGet(GROUND_SCALE, GROUND_SCALE, GROUND_SCALE));
}

void Ground::DrawGround() const {

    // X軸ループ
    for (int x = -GameConfig::GRID_HALF_NUM; x <= GameConfig::GRID_HALF_NUM; x++)
    {
        // X座標をマス間隔を計算して設定
        float xPos = x * GameConfig::GRID_SIZE;

        // Z軸ループ
        for (int z = -GameConfig::GRID_HALF_NUM; z <= GameConfig::GRID_HALF_NUM; z++)
        {
            // Z座標をマス間隔を計算して設定
            float zPos = z * GameConfig::GRID_SIZE;

            // x,z共に0の部分を原点にする(赤色で描画)
            if (x == 0 && z == 0)
            {
                // 赤色で描画
                MV1SetDifColorScale(groundHandle, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
            }
            else
            {
                // モデルの原色で描画
                MV1SetDifColorScale(groundHandle, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
            }

            // モデルを座標にセット
            MV1SetPosition(groundHandle, VGet(xPos, GROUND_Y, zPos));

            // 地面モデル描画
            MV1DrawModel(groundHandle);
        }
    }
}