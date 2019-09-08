#include "MiterBoard.h"

void MiterBoard::Init()
{
	Image.Init(IMAGENUM_MITER, 1, 86, 30);
}
void MiterBoard::Draw(int distance, int x, int y, int scrollSpeedX, int scrollSpeedY) //offset�� �ʱ�ȭ �Ǹ� true��ȯ
{
	Image.DrawMoveBackground(ResourceManager::backBuffer->GetmemDC(), x, y, scrollSpeedX);
	_font.moveDraw(distance, x, y, scrollSpeedX, 0, -86); //���� �����̹��� ũ��
}

MiterBoard::MiterBoard(){}
MiterBoard::~MiterBoard(){ }