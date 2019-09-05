#pragma once
#include<Windows.h>
#include"ResourceManager.h"

enum SPRITEIMAGES
{
	PLAYER0,
	PLAYER1,
	PLAYER2,
	ENDSPRITE
};
class SpriteRenderer
{
private:
	int backgroundOffsetX[10] = { 0 };
	int backgroundOffsetY[10] = { 0 };
	SPRITEIMAGES spriteImages;
	int scrollSpeed[10];
	int backgroundImageSizeX[10];
	int backgroundImageSizeY[10];
public:
	void Init();
	SpriteRenderer();
	~SpriteRenderer();
	void DrawPlayer(HDC hdc, int playerX, int PlayerY, int playerSizeX, int playerSizeY);
	void DrawBackground(HDC hdc, int backGroundImageSizeX, int backGroundImageSizeY);
};