#include "SpriteRenderer.h"

SpriteRenderer* SpriteRenderer::mthis = nullptr;

SpriteRenderer::SpriteRenderer(){ }
SpriteRenderer::~SpriteRenderer(){ }

void SpriteRenderer::Init()
{
	//�̹����� ������ ���� ���η��̵��ϸ� ����ũ��, ���η��̵��ϸ� ����ũ�� �ʿ�
	ImageSizeX[0] = 80;
	ImageSizeY[0] = 100;
	scrollSpeedX[0] = 1;
}
//Ÿ�̸ӷ� Ư�� ���� ���� ȣ�� (�̹��� ������ ������)
void SpriteRenderer::DrawPlayer(HDC hdc, int playerX, int PlayerY, int playerSizeX, int playerSizeY)
{
	ResourceManager::GetInstance()->Draw(hdc, playerX, PlayerY, playerSizeX, playerSizeY, (IMAGENUM)(IMAGENUM_PLAYER_1 + spriteImages));

	if (spriteImages == ENDSPRITE)
		spriteImages = PLAYER0;
	else
		spriteImages = (SPRITEIMAGES)(spriteImages + 1);
}
void SpriteRenderer::DrawBackground(HDC hdc, int backGroundImageSizeX, int backGroundImageSizeY)
{
	//���� ��� ������� 0���̹����� ���� ����°ɷ� ����
	for (int i = -5; i < 20; i++)
	{
		//�ܼ� �ݺ� ���
		ResourceManager::GetInstance()->Draw(hdc, i * 67, 535, ImageSizeX[0], ImageSizeY[0], IMAGENUM_BACK);
	}
	//ResourceManager::GetInstance()->Draw(hdc, i * 64 + backgroundOffsetX[0], 470, 65, 64, BACKNORMAL);
	
	//���� ��ǥ�� ���� ��
	ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX[0], backgroundOffsetY[0], ImageSizeX[0], ImageSizeY[0], IMAGENUM_BACK);
	//�׸��� �������� �̵���
	for (size_t i = 0; i < 10; i++)
	{
		backgroundOffsetX[0] -= scrollSpeedX[0];
		if (backgroundOffsetX[0] >= ImageSizeX[0])
		{
			backgroundOffsetX[0] = 0;
		}
	}
}