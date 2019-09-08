#pragma once
#include<Windows.h>
#include"ResourceManager.h"
#include"Physics.h"

class SpriteRenderer
{
private:
	int backgroundOffsetX = 0;
	int backgroundOffsetY = 0;

	IMAGENUM startSprite; //���ҽ����� �����̹��� �ε���
	IMAGENUM CurSprite;
	int AllSpriteNum; //�� ���ҽ� ����

	int ImageSizeX;
	int ImageSizeY;
public:
	void Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY);
	SpriteRenderer();
	~SpriteRenderer();
	void DrawObject(HDC hdc, int playerX, int PlayerY);
	void SpriteChange();
	void DrawBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum);
	bool DrawMoveBackground(HDC hdc, int objectX, int objectY, int scrollSpeedX = 0, int scrollSpeedY = 0);
	void DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int _scrollSpeedX = 0, int _scrollSpeedY = 0);
};
