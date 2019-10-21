#pragma once
#include<Windows.h>
#include"ResourceManager.h"

class SpriteRenderer
{
protected:
	int backgroundOffsetX = 0;
	int backgroundOffsetY = 0;

	int startSprite; //���ҽ����� �����̹��� �ε���

	int ImageSizeX;
	int ImageSizeY;
	int defaultX;
	int defaultY;

	//������
	int CurSprite;
	int AllSpriteNum; //�� ���ҽ� ����
public:
	void Init(int _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);
	// void Init(int _startSprite, int _ImageSizeX, int _ImageSizeY, int objectX = 0, int objectY = 0);

	//���� Ŭ���� �ʱ�ȭ
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