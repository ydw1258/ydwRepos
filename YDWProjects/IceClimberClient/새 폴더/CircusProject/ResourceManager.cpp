#include "ResourceManager.h"

ResourceManager* ResourceManager::mthis = nullptr;
BitMap* ResourceManager::backBuffer = nullptr;

void ResourceManager::Init(HDC hdc, HINSTANCE hInstance)
{
	RECT rc;
	
	bitmap = new BitMap[30];
	backBuffer = bitmap[0].Create(hdc, 1024, 720);

	bitmap[1].Init(hdc, hInstance, "back.bmp");
	bitmap[2].Init(hdc, hInstance, "back_deco.bmp");
	bitmap[3].Init(hdc, hInstance, "back_normal.bmp");
	bitmap[4].Init(hdc, hInstance, "back_normal2.bmp");
	bitmap[5].Init(hdc, hInstance, "cash.bmp");
	bitmap[6].Init(hdc, hInstance, "die.bmp");
	bitmap[7].Init(hdc, hInstance, "end.bmp");
	bitmap[8].Init(hdc, hInstance, "enemy.bmp");
	bitmap[9].Init(hdc, hInstance, "enemy_1b.bmp");
	bitmap[10].Init(hdc, hInstance, "enemy_1f.bmp");
	bitmap[11].Init(hdc, hInstance, "enemy_b.bmp");
	bitmap[12].Init(hdc, hInstance, "enemy_f.bmp");
	bitmap[13].Init(hdc, hInstance, "enemy_l.bmp");
	bitmap[14].Init(hdc, hInstance, "enemy_l_b.bmp");
	bitmap[15].Init(hdc, hInstance, "enemy_l_f.bmp");
	bitmap[16].Init(hdc, hInstance, "enemy1.bmp");
	bitmap[17].Init(hdc, hInstance, "front.bmp");
	bitmap[18].Init(hdc, hInstance, "front2.bmp");
	bitmap[19].Init(hdc, hInstance, "icon.bmp");
	bitmap[20].Init(hdc, hInstance, "miter.bmp");
	bitmap[21].Init(hdc, hInstance, "player0.bmp");
	bitmap[22].Init(hdc, hInstance, "player1.bmp");
	bitmap[23].Init(hdc, hInstance, "player2.bmp");
	bitmap[24].Init(hdc, hInstance, "star.bmp");
	bitmap[25].Init(hdc, hInstance, "star1.bmp");
	bitmap[26].Init(hdc, hInstance, "star2.bmp");
	bitmap[27].Init(hdc, hInstance, "win.bmp");
	bitmap[28].Init(hdc, hInstance, "win2.bmp");

	AddFontResourceA("godoMaum.ttf");
}

void ResourceManager::Draw(HDC hdc, int x, int y, int cx, int cy, IMAGENUM imageNum)
{
	bitmap[imageNum].Draw(hdc, x, y, cx, cy);
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Release()
{
	delete[] bitmap;
	delete mthis;
}