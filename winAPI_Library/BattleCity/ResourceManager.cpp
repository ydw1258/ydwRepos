#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	string fileNames[50] = {"BattleResource/block00.bmp", "BattleResource/block01.bmp","BattleResource/block02.bmp", "BattleResource/block03.bmp", "BattleResource/block04.bmp", "BattleResource/block05.bmp", "BattleResource/block06.bmp", "BattleResource/block07.bmp", 
	"BattleResource/block08.bmp", "BattleResource/block09.bmp", "BattleResource/block10.bmp", "BattleResource/block11.bmp", "BattleResource/block12.bmp", 
	"BattleResource/block13.bmp", "BattleResource/block14.bmp", "BattleResource/e_down_00.bmp", "BattleResource/e_down_01.bmp", "BattleResource/e_down_02.bmp",
		"BattleResource/e_left_00.bmp", 		"BattleResource/e_left_01.bmp",		"BattleResource/e_left_02.bmp",	"BattleResource/e_right_00.bmp", "BattleResource/e_right_01.bmp",
		"BattleResource/e_right_02.bmp", "BattleResource/e_up_00.bmp",		"BattleResource/e_up_01.bmp",		"BattleResource/e_up_02.bmp", "BattleResource/e_enemy_icon.bmp",

		"BattleResource/tank_down_00.bmp",
		"BattleResource/tank_down_01.bmp",
		"BattleResource/tank_down_02.bmp",
		"BattleResource/tank_left_00.bmp",
		"BattleResource/tank_left_01.bmp",
		"BattleResource/tank_left_02.bmp",
		"BattleResource/tank_right_00.bmp",
		"BattleResource/tank_right_01.bmp",
		"BattleResource/tank_right_02.bmp",
		"BattleResource/tank_up_00.bmp",
		"BattleResource/tank_up_01.bmp",
		"BattleResource/tank_up_02.bmp", // 41
	};

	bitmap = new BitMap[50];
	string blackBackStr = "BattleResource/battlecityBack";

	backBuffer = bitmap[0].Create(hdc, 600, 600);
	//backBuffer = bitmap[0].Init(hdc, hInstance, blackBackStr.c_str());
	for(int i = 1; i <= 40; i++)
		bitmap[i].Init(hdc, hInstance, fileNames[i].c_str());
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}
ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

void ResourceManager::Release()
{
	delete[] bitmap;
	delete mthis;
}