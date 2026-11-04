#include <cmath>
#include "Ground.h"
#include "DxLib.h"

Ground::Ground() {

    // 地面モデル読み込み
    groundHandle = MV1LoadModel("Stage/Grounds.mv1");

    // 地面モデルのスケールを設定
    MV1SetScale(groundHandle, VGet(GROUND_SCALE, GROUND_SCALE, GROUND_SCALE));

    // 初期テクスチャを取得
    groundNormalHandle = MV1GetTextureGraphHandle(groundHandle, 0);

    // 各地形タイプのテクスチャを取得
    groundGrassHandle = LoadGraph("Stage/GrassTexture.jpg");
    groundWaterHandle = LoadGraph("Stage/IceTexture.jpg");
    groundSandHandle = LoadGraph("Stage/SandTexture.jpg");

    // 乱数エンジンを初期化
    std::random_device rd;
    rng = std::mt19937(rd());

    // 特定座標のマスにタイプ(テクスチャ)を設定
    /*SetType(-3, 3, GameConfig::GroundType::Grass);
    SetType(-3, -3, GameConfig::GroundType::Grass);
    SetType(3, 3, GameConfig::GroundType::Grass);
    SetType(3, -3, GameConfig::GroundType::Grass);
    SetType(-1, 1, GameConfig::GroundType::Sand);
    SetType(-1, -1, GameConfig::GroundType::Sand);
    SetType(1, 1, GameConfig::GroundType::Water);
    SetType(1, -1, GameConfig::GroundType::Water);*/

    // 地形タイプをランダムに設定
    GroundRandomSet();
}

// 全マスを地形タイプに応じたテクスチャで描画する
void Ground::DrawGround() const {

    // 全マスを走査(x * z)
    for (int x = -GameConfig::GRID_HALF_NUM; x <= GameConfig::GRID_HALF_NUM; x++)
    {
        float xPos = x * GameConfig::GRID_SIZE;

        for (int z = -GameConfig::GRID_HALF_NUM; z <= GameConfig::GRID_HALF_NUM; z++)
        {
            float zPos = z * GameConfig::GRID_SIZE;

            // 設定されているタイプを取得
            GameConfig::GroundType type = GetType(x, z);

            // タイプごとにテクスチャを設定
            switch (type)
            {
                case GameConfig::GroundType::Normal:
                {
                    MV1SetTextureGraphHandle(groundHandle, 0, groundNormalHandle, FALSE);

                    break;
                }

                case GameConfig::GroundType::Grass:
                {
                    MV1SetTextureGraphHandle(groundHandle, 0, groundGrassHandle, FALSE);

                    break;
                }

                case GameConfig::GroundType::Water:
                {
                    MV1SetTextureGraphHandle(groundHandle, 0, groundWaterHandle, FALSE);

                    break;
                }

                case GameConfig::GroundType::Sand:
                {
                    MV1SetTextureGraphHandle(groundHandle, 0, groundSandHandle, FALSE);

                    break;
                }
            }

            // モデルを座標にセット
            MV1SetPosition(groundHandle, VGet(xPos, GROUND_Y, zPos));

            // 地面モデル描画
            MV1DrawModel(groundHandle);
        }
    }
}

// マスのタイプごとの移動速度倍率を返す
float Ground::GetSpeedMultiplierAtWorldPos(float worldX, float worldZ) const {

    GameConfig::GroundType type = GetTypeAtWorldPos(worldX, worldZ);

    return speedMultiplierByType[static_cast<int>(type)];
}

// マスのタイプに応じたエフェクトの色を返す
unsigned int Ground::GetGroundEffectColor(float worldX, float worldZ) const {

    GameConfig::GroundType type = GetTypeAtWorldPos(worldX, worldZ);

    unsigned int color = 0;

    switch (type)
    {
    case GameConfig::GroundType::Grass:

        color = GetColor(80, 180, 60);

        break;

    case GameConfig::GroundType::Water:

        color = GetColor(0, 0, 255);

        break;

    case GameConfig::GroundType::Sand:

        color = GetColor(222, 184, 135);

        break;
    }

    return color;
}

// マス番号のタイプを返す
GameConfig::GroundType Ground::GetType(int x, int z) const {

    if (IsOutOfRange(x) || IsOutOfRange(z)) return GameConfig::GroundType::Normal;

    // 各マスのタイプを返す
    return grids[ToIndex(x)][ToIndex(z)];
}

void Ground::SetType(int x, int z, GameConfig::GroundType type) {

    if (IsOutOfRange(x) || IsOutOfRange(z)) return;

    // 指定したマスにタイプを設定
    grids[ToIndex(x)][ToIndex(z)] = type;
}



void Ground::GroundRandomSet() {

    std::uniform_int_distribution<int> randGroundType(1, 100);

    for (int x = -GameConfig::GRID_HALF_NUM; x <= GameConfig::GRID_HALF_NUM; x ++)
    {
        for (int z = -GameConfig::GRID_HALF_NUM; z <= GameConfig::GRID_HALF_NUM; z++)
        {
            int randomNum = randGroundType(rng);

            GameConfig::GroundType type = NumToType(randomNum);

            SetType(x, z, type);
        }
    }

    // スポーン位置はノーマルタイプ固定
    SetType(0, 0, GameConfig::GroundType::Normal);
}

// 乱数を元に地形タイプを返す
GameConfig::GroundType Ground::NumToType(int num) {

    // 累積値を計算(normalは normalMaxまで, grassは normalMax + grassまで)
    int normalMax = groundRandomPercent[0];
    int grassMax = normalMax + groundRandomPercent[1];
    int waterMax = grassMax + groundRandomPercent[2];

    if (num <= normalMax) return GameConfig::GroundType::Normal;
    if (num <= grassMax) return GameConfig::GroundType::Grass;
    if (num <= waterMax) return GameConfig::GroundType::Water;
    return GameConfig::GroundType::Sand;
}


// 範囲外チェック
bool Ground::IsOutOfRange(int pos) const {

    return (pos < -GameConfig::GRID_HALF_NUM || pos > GameConfig::GRID_HALF_NUM);
}

// ワールド座標(中心0基準: -HALF～+HALF) を 配列添え字に変換
int Ground::ToIndex(int pos) const {

    return pos + GameConfig::GRID_HALF_NUM;
}

// 対象が立っているマスタイプを返す
GameConfig::GroundType Ground::GetTypeAtWorldPos(float worldX, float worldZ) const {

    // 現在座標から近くのマス番号を計算
    int xNum = static_cast<int>(std::round(worldX / GameConfig::GRID_SIZE));
    int zNum = static_cast<int>(std::round(worldZ / GameConfig::GRID_SIZE));

    return GetType(xNum, zNum);
}
