#include "SpriteRenderer.h"
#include "GameManager.h"
#include<iostream>
using namespace std;

SpriteRenderer::SpriteRenderer() { }
SpriteRenderer::~SpriteRenderer() { }

//ȭ�鳡���� �������� �ݺ� ����� ��� objectX, objectY �ʿ�.
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
//Ÿ�̸ӷ� Ư�� ���� ���� ȣ�� (�̹��� ������ ������)
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
	//���� ��� �������
	for (int i = -1; i < repeatYNum; i++)
	{
		for (int j = -1; j < repeatXNum; j++)
		{
			ResourceManager::GetInstance()->Draw(hdc, j * GameManager::GetInstance()->CameraX % ImageSizeX, objectY * i, ImageSizeX, ImageSizeY, CurSprite);
		}
	}
}
//������
//ȭ�� �Ѿ�� �ٽ� ������ ��ǥ��
void SpriteRenderer::DrawMoveBackground(HDC hdc, int objectX, int objectY)
{
	ResourceManager::GetInstance()->Draw(hdc, objectX - GameManager::GetInstance()->CameraX % (ImageSizeX * 16), objectY, ImageSizeX, ImageSizeY, CurSprite);
}
void SpriteRenderer::DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int scrollSpeedX, int scrollSpeedY)
{
	//���� ��ǥ�� ���� ��
	//ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX, backgroundOffsetY, ImageSizeX, ImageSizeY, CurSprite);
	//�׸��� �������� �̵���

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