#include "Game.h"
#include "GameConfig.h"
#include "DxLib.h"

// ÉvÉçÉOÉâÉÄÇÕ WinMain Ç©ÇÁénÇ‹ÇËÇ‹Ç∑
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ChangeWindowMode(TRUE);
    SetGraphMode(GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT, 32);
    if (DxLib_Init() < 0) return -1;


    {
        Game game;

        game.Run();
    }
    

    DxLib_End();
    return 0;
}