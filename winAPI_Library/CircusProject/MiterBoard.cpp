#include "MiterBoard.h"

void MiterBoard::Init()
{
	Image.Init(IMAGENUM_MITER, 1, 86, 30);
	//_font.Init();
}
void MiterBoard::Draw(int distance, int x, int y, int scrollSpeedX, int scrollSpeedY)
{
	Image.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), x, y, scrollSpeedX);
	_font.moveDraw(distance, x, y, scrollSpeedX);
}

MiterBoard::MiterBoard()
{
}

MiterBoard::~MiterBoard()
{
}
