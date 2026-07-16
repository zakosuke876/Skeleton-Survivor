#include "Sword.h"
#include "Player.h"

Sword::Sword() {

	swordHandle = MV1LoadModel("Model/Sword/Player_Sword.mv1");
	swordTexHandle = LoadGraph("Model/Player/knight_texture.png");
	MV1SetTextureGraphHandle(swordHandle, 0, swordTexHandle, FALSE);
}

Sword::~Sword() {

	// リソース開放
	MV1DeleteModel(swordHandle);
	DeleteGraph(swordTexHandle);
}

void Sword::SetUpBone(int playerModelHandle) {

	handFrameIndex = MV1SearchFrame(playerModelHandle, "handslot.r");
}

void Sword::Update(int playerModelHandle) {

	MATRIX handMatrix = MV1GetFrameLocalWorldMatrix(playerModelHandle, handFrameIndex);
	MATRIX offset = MGetScale(VGet(SWORD_SCALE, SWORD_SCALE, SWORD_SCALE));
	MATRIX swordMatrix = MMult(offset, handMatrix);
	MV1SetMatrix(swordHandle, swordMatrix);

	swordPos = VTransform(VGet(0, SWORD_TIP_OFFSET, 0), swordMatrix);
}

void Sword::Draw() const {

	MV1DrawModel(swordHandle);

	DrawSphere3D(swordPos, 1.0f, 80, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
}