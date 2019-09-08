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
//Ÿ�̸ӷ� Ư�� ���� ���� ȣ�� (�̹��� ������ ������)
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
	//���� ��� �������
	for (int i = 0; i < repeatYNum; i++)
	{
		for (int j = 0; j < repeatXNum; j++)
		{
			//�ܼ� �ݺ� ���
			DrawObject(hdc, objectX + j * ImageSizeX, objectY + ImageSizeY * i);
		}
	}
}
bool SpriteRenderer::DrawMoveBackground(HDC hdc, int objectX, int objectY, int scrollSpeedX, int scrollSpeedY) //�̹��� �������� �ʱ�ȭ�Ǹ� true��ȯ, �ӽ�..
{
	DrawObject(hdc, objectX - backgroundOffsetX, objectY - backgroundOffsetY);
	//cout << scrollSpeedX << endl;

	backgroundOffsetX -= scrollSpeedX;
	backgroundOffsetY -= scrollSpeedY;

	if (objectX - backgroundOffsetX + ImageSizeX < 0)
	{
		backgroundOffsetX = 0;
		GameManager::GetInstance()->distance -= 10; //�� ����
		return true;
	}
		
	if (objectX - backgroundOffsetX + ImageSizeY < 0)
		backgroundOffsetY = 0;
	return false;
}
void SpriteRenderer::DrawSrolledBackground(HDC hdc, int objectX, int objectY, int repeatXNum, int repeatYNum, int scrollSpeedX, int scrollSpeedY)
{
	//���� ��ǥ�� ���� ��
	//ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX, backgroundOffsetY, ImageSizeX, ImageSizeY, CurSprite);
	//�׸��� �������� �̵���
	
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