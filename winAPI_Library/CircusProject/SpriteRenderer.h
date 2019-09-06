#pragma once
#include<Windows.h>
#include"ResourceManager.h"
#include"Physics.h"
class SpriteRenderer
{
private:
	int backgroundOffsetX = 0;
	int backgroundOffsetY = 0;

	IMAGENUM startSprite; //리소스에서 시작이미지 인덱스
	IMAGENUM CurSprite;
	int AllSpriteNum; //총 리소스 갯수

	int scrollSpeedX;
	int scrollSpeedY;
	int ImageSizeX;
	int ImageSizeY;
public:
	void Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, 
		int _backgroundOffsetX = 0, int _backgroundOffsetY = 0, int _scrollSpeedX = 0, int _scrollSpeedY = 0);
	SpriteRenderer();
	~SpriteRenderer();
	void DrawObject(HDC hdc, int playerX, int PlayerY);
	void DrawBackground(HDC hdc, int repeatXNum, int repeatYNum);
	void DrawSrolledBackground(HDC hdc, int repeatXNum, int repeatYNum);
};

#pragma once

