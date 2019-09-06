#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() { }
SpriteRenderer::~SpriteRenderer() { }

void SpriteRenderer::Init(IMAGENUM _startSprite, int _AllSpriteNum, int _ImageSizeX, int _ImageSizeY, int _backgroundOffsetX, int _backgroundOffsetY, int _scrollSpeedX , int _scrollSpeedY)
{
	startSprite = _startSprite;
	CurSprite = _startSprite;
	AllSpriteNum = _AllSpriteNum;
	ImageSizeX = _ImageSizeX;
	ImageSizeY = _ImageSizeY;
	backgroundOffsetX = _backgroundOffsetX;
	backgroundOffsetY = _backgroundOffsetY;
	scrollSpeedX = _scrollSpeedX;
	scrollSpeedY = _scrollSpeedY;
}
//타이머로 특정 간격 마다 호출 (이미지 변경의 시점만)
void SpriteRenderer::DrawObject(HDC hdc, int playerX, int PlayerY)
{
	ResourceManager::GetInstance()->Draw(hdc, playerX, PlayerY, ImageSizeX, ImageSizeY, CurSprite);
	if (CurSprite == startSprite + AllSpriteNum - 1)
		CurSprite = startSprite;
	else
		CurSprite = (IMAGENUM)(CurSprite + 1);

}
void SpriteRenderer::DrawBackground(HDC hdc, int repeatXNum, int repeatYNum)
{
	//같은 배경 여럿찍기
	for (int i = 0; i < repeatYNum; i++)
	{
		for (int j = 0; j < repeatXNum; j++)
		{
			//단순 반복 배경
			ResourceManager::GetInstance()->Draw(hdc, i * ImageSizeX, ImageSizeY * j, ImageSizeX, ImageSizeY, CurSprite);
		}
	}
	//ResourceManager::GetInstance()->Draw(hdc, i * 64 + backgroundOffsetX[0], 470, 65, 64, BACKNORMAL);

}
void SpriteRenderer::DrawSrolledBackground(HDC hdc, int repeatXNum, int repeatYNum)
{
	//시작 좌표를 당기는 것
	ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX, backgroundOffsetY, ImageSizeX, ImageSizeY, CurSprite);
	
	//그림이 왼쪽으로 이동시
	for (int i = 0; i < repeatYNum; i++)
	{
		for (int j = 0; j < repeatXNum; j++)
		{
			ResourceManager::GetInstance()->Draw(hdc, i * ImageSizeX - scrollSpeedX, ImageSizeY * j - scrollSpeedY, ImageSizeX, ImageSizeY, CurSprite);
		}
	}
	backgroundOffsetX -= scrollSpeedX;
	backgroundOffsetY -= scrollSpeedY;

	if (backgroundOffsetX >= ImageSizeX && scrollSpeedX < 0||
		backgroundOffsetX <= ImageSizeX && scrollSpeedX > 0)
		backgroundOffsetX = 0;
	if (backgroundOffsetY >= ImageSizeY && scrollSpeedY < 0 ||
		backgroundOffsetY <= ImageSizeY && scrollSpeedY > 0)
		backgroundOffsetY = 0;
}