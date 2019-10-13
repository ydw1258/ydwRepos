#include "SpriteRenderer.h"
#include "GameManager.h"
#include<iostream>
using namespace std;

SpriteRenderer::SpriteRenderer() { }
SpriteRenderer::~SpriteRenderer() { }

//화면끝에서 없어지는 반복 배경일 경우 objectX, objectY 필요.
void SpriteRenderer::Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int objectX, int objectY)
{
	startSprite = _startSprite;
	CurSprite = _startSprite;
	AllSpriteNum = _AllSpriteNum;
	ImageSizeX = _ImageSizeX;
	ImageSizeY = _ImageSizeY;
	defaultX = objectX + ImageSizeX - (GameManager::GetInstance()->CameraX % ImageSizeX);
	//defaultY = objectY + ImageSizeY - (GameManager::GetInstance()->CameraY % ImageSizeY);
}
//타이머로 특정 간격 마다 호출 (이미지 변경의 시점만)
void SpriteRenderer::DrawObject(HDC hdc, int objectX, int objectY)
{
	ResourceManager::GetInstance()->Draw(hdc, objectX - GameManager::GetInstance()->CameraX, objectY, ImageSizeX, ImageSizeY, CurSprite);
}
void SpriteRenderer::SpriteChange()
{
	if (CurSprite == startSprite + AllSpriteNum - 1)
		CurSprite = startSprite;
	else
		CurSprite = (IMAGENUM)(CurSprite + 1);
}
void SpriteRenderer::DrawBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum)
{
	//같은 배경 여럿찍기
	for (int i = -1; i < repeatYNum; i++)
	{
		for (int j = -1; j < repeatXNum; j++)
		{
			ResourceManager::GetInstance()->Draw(hdc, j * GameManager::GetInstance()->CameraX % ImageSizeX, objectY * i, ImageSizeX, ImageSizeY, CurSprite);
		}
	}
}
//보수중
//화면 넘어가면 다시 원래의 좌표로
void SpriteRenderer::DrawMoveBackground(HDC hdc, int objectX, int objectY)
{
	ResourceManager::GetInstance()->Draw(hdc, objectX - GameManager::GetInstance()->CameraX % (ImageSizeX * 16), objectY, ImageSizeX, ImageSizeY, CurSprite);
}
void SpriteRenderer::DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int scrollSpeedX, int scrollSpeedY)
{
	//시작 좌표를 당기는 것
	//ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX, backgroundOffsetY, ImageSizeX, ImageSizeY, CurSprite);
	//그림이 왼쪽으로 이동시

	for (int i = 1; i < repeatYNum + 1; i++)
	{
		for (int j = -1; j < repeatXNum - 1; j++)
		{
			ResourceManager::GetInstance()->Draw(hdc, objectX + j * ImageSizeX - (GameManager::GetInstance()->CameraX % ImageSizeX), objectY * i, ImageSizeX, ImageSizeY, CurSprite);
		}
	}
	//cout << scrollSpeedX << endl;

	backgroundOffsetX -= scrollSpeedX;
	backgroundOffsetY -= scrollSpeedY;

	if (backgroundOffsetX >= ImageSizeX && scrollSpeedX < 0 ||
		backgroundOffsetX <= -ImageSizeX && scrollSpeedX > 0)
		backgroundOffsetX = 0;

	if (backgroundOffsetY >= ImageSizeY && scrollSpeedY < 0 ||
		backgroundOffsetY <= ImageSizeY && scrollSpeedY > 0)
		backgroundOffsetY = 0;
}