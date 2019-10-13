#pragma once
#include<Windows.h>
#include"ResourceManager.h"

class SpriteRenderer
{
private:
	int backgroundOffsetX = 0;
	int backgroundOffsetY = 0;

	IMAGENUM startSprite; //리소스에서 시작이미지 인덱스
	IMAGENUM CurSprite;
	int AllSpriteNum; //총 리소스 갯수

	int ImageSizeX;
	int ImageSizeY;
	int defaultX;
	int defaultY;
public:
	void Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);
	SpriteRenderer();
	~SpriteRenderer();
	void DrawObject(HDC hdc, int playerX, int PlayerY);
	void SpriteChange();
	void DrawBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum);
	void DrawMoveBackground(HDC hdc, int objectX, int objectY);
	void DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int _scrollSpeedX = 0, int _scrollSpeedY = 0);
};