#include "SpriteRenderer.h"
#include "GameManager.h"
#include<iostream>
using namespace std;

SpriteRenderer::SpriteRenderer() { }
SpriteRenderer::~SpriteRenderer() { }

void SpriteRenderer::Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY)
{
	startSprite = _startSprite;
	CurSprite = _startSprite;
	AllSpriteNum = _AllSpriteNum;
	ImageSizeX = _ImageSizeX;
	ImageSizeY = _ImageSizeY;
}
//타이머로 특정 간격 마다 호출 (이미지 변경의 시점만)
void SpriteRenderer::DrawObject(HDC hdc, int objectX, int objectY)
{
 	ResourceManager::GetInstance()->Draw(hdc, objectX, objectY, ImageSizeX, ImageSizeY, CurSprite);
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
	for (int i = 0; i < repeatYNum; i++)
	{
		for (int j = 0; j < repeatXNum; j++)
		{
			//단순 반복 배경
			DrawObject(hdc, objectX + j * ImageSizeX, objectY + ImageSizeY * i);
		}
	}
}
bool SpriteRenderer::DrawMoveBackground(HDC hdc, int objectX, int objectY, int scrollSpeedX, int scrollSpeedY) //이미지 오프셋이 초기화되면 true반환, 임시..
{
	DrawObject(hdc, objectX - backgroundOffsetX, objectY - backgroundOffsetY);
	//cout << scrollSpeedX << endl;

	backgroundOffsetX -= scrollSpeedX;
	backgroundOffsetY -= scrollSpeedY;

	if (objectX - backgroundOffsetX + ImageSizeX < 0)
	{
		backgroundOffsetX = 0;
		GameManager::GetInstance()->distance -= 10; //뺄 예정
		return true;
	}
		
	if (objectX - backgroundOffsetX + ImageSizeY < 0)
		backgroundOffsetY = 0;
	return false;
}
void SpriteRenderer::DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int scrollSpeedX, int scrollSpeedY)
{
	//시작 좌표를 당기는 것
	//ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX, backgroundOffsetY, ImageSizeX, ImageSizeY, CurSprite);
	//그림이 왼쪽으로 이동시
	
	for (int i = 0; i < repeatYNum; i++)
	{
		for (int j = 0; j < repeatXNum; j++)
		{
			DrawObject(hdc, objectX + (j - 1) * ImageSizeX - backgroundOffsetX - ImageSizeX, objectY + ImageSizeY * i - backgroundOffsetY);
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