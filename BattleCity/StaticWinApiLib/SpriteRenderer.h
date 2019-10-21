#pragma once
#include<Windows.h>
#include"ResourceManager.h"

class SpriteRenderer
{
protected:
	int backgroundOffsetX = 0;
	int backgroundOffsetY = 0;

	int startSprite; //리소스에서 시작이미지 인덱스

	int ImageSizeX;
	int ImageSizeY;
	int defaultX;
	int defaultY;

	//내릴거
	int CurSprite;
	int AllSpriteNum; //총 리소스 갯수
public:
	void Init(int _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);
	// void Init(int _startSprite, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);

	//내릴 클래스 초기화
	//void Init(int _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);
	SpriteRenderer();
	~SpriteRenderer();
	void DrawObject(HDC hdc, int objectX, int objectY);
	
	void DrawBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum);
	void DrawMoveBackground(HDC hdc, int objectX, int objectY);
	void DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int _scrollSpeedX = 0, int _scrollSpeedY = 0);
	
	void DrawResizedObject(HDC hdc, int objectX, int objectY, int sizeX, int sizeY);
	void SpriteChange();
};