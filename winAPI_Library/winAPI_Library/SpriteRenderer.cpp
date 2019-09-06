#include "SpriteRenderer.h"

SpriteRenderer* SpriteRenderer::mthis = nullptr;

SpriteRenderer::SpriteRenderer(){ }
SpriteRenderer::~SpriteRenderer(){ }

void SpriteRenderer::Init()
{
	//이미지별 오프셋 설정 가로로이동하면 가로크기, 세로로이동하면 세로크기 필요
	ImageSizeX[0] = 80;
	ImageSizeY[0] = 100;
	scrollSpeedX[0] = 1;
}
//타이머로 특정 간격 마다 호출 (이미지 변경의 시점만)
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
	//같은 배경 여럿찍기 0번이미지를 여러 번찍는걸로 예시
	for (int i = -5; i < 20; i++)
	{
		//단순 반복 배경
		ResourceManager::GetInstance()->Draw(hdc, i * 67, 535, ImageSizeX[0], ImageSizeY[0], IMAGENUM_BACK);
	}
	//ResourceManager::GetInstance()->Draw(hdc, i * 64 + backgroundOffsetX[0], 470, 65, 64, BACKNORMAL);
	
	//시작 좌표를 당기는 것
	ResourceManager::GetInstance()->Draw(hdc, backgroundOffsetX[0], backgroundOffsetY[0], ImageSizeX[0], ImageSizeY[0], IMAGENUM_BACK);
	//그림이 왼쪽으로 이동시
	for (size_t i = 0; i < 10; i++)
	{
		backgroundOffsetX[0] -= scrollSpeedX[0];
		if (backgroundOffsetX[0] >= ImageSizeX[0])
		{
			backgroundOffsetX[0] = 0;
		}
	}
}