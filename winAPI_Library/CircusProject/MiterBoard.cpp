#include "MiterBoard.h"
#include "GameManager.h"

void MiterBoard::Init()
{
	Image.Init(IMAGENUM_MITER, 1, 86, 30);
}
void MiterBoard::Draw(int distance, int x, int y, int scrollSpeedX, int scrollSpeedY) //offset�� �ʱ�ȭ �Ǹ� true��ȯ
{
	Image.DrawObject(ResourceManager::backBuffer->GetmemDC(), x, y);
	_font.Draw(distance, x + 30, y); //���� �����̹��� ũ��
}

MiterBoard::MiterBoard(){}
MiterBoard::~MiterBoard(){ }