#include "MiterBoard.h"

void MiterBoard::Init()
{
	Image.Init(IMAGENUM_MITER, 1, 86, 30);
}
void MiterBoard::Draw(int distance, int x, int y, int scrollSpeedX, int scrollSpeedY) //offset이 초기화 되면 true반환
{
	Image.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), x, y, scrollSpeedX);
	_font.moveDraw(distance, x, y, scrollSpeedX, 0, -86); //미터 가로이미지 크기
}

MiterBoard::MiterBoard(){}
MiterBoard::~MiterBoard(){ }