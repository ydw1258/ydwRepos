#include "MineButton.h"
#include "ResManager.h"
#include "MainGame.h"
#include <ctime>
#include<iostream>
using namespace std;

MineButton::MineButton()
{
}
MineButton::~MineButton()
{
}
void MineButton::Init(int x, int y)
{
	isBoom = rand() % 10 == 0;

	resourceName = BLOCK;
	m_iX = x;
	m_iY = y;
	
	for (int offsetY = -1; offsetY <= 1; offsetY++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX++)
		{
			if (offsetX == 0 && offsetY == 0)
				continue;

			if (m_iX + offsetX < 0 || m_iX + offsetX >= 10)
			{
				continue;
			}
			if (m_iY + offsetY < 0 || m_iY + offsetY >= 10)
			{
				continue;
			}
			vNeighbor.push_back( POINT{ m_iX + offsetX,m_iY + offsetY });
		}
	}
}

void MineButton::Draw(HDC hdc)
{
	switch (resourceName)
	{
	case BLOCK:
		ResManager::inst()->GetBitMap("block.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK00:
		ResManager::inst()->GetBitMap("block_0.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK01:
		ResManager::inst()->GetBitMap("block_1.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK02:
		ResManager::inst()->GetBitMap("block_2.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK03:
		ResManager::inst()->GetBitMap("block_3.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK04:
		ResManager::inst()->GetBitMap("block_4.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK05:
		ResManager::inst()->GetBitMap("block_5.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK06:
		ResManager::inst()->GetBitMap("block_6.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK07:
		ResManager::inst()->GetBitMap("block_7.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BLOCK08:
		ResManager::inst()->GetBitMap("block_8.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case FLAG:
		break;
	case MINE:
		ResManager::inst()->GetBitMap("mine.bmp")->Draw(hdc, m_iX * 25, m_iY * 25, 0, 0);
		break;
	case BACK:
		break;
	default:
		break;
	}
}

bool MineButton::Input(POINT pt)
{
	//if (m_bOpen)
	RECT rc = { m_iX *25 , m_iY *25 , m_iX*25 + 25  , m_iY*25 + 25 };
	int count = 0;
	
	if (PtInRect(&rc, pt))
	{
		if (isBoom)
		{
			resourceName = MINE;
			return false;
		}
		//for (auto n : vNeighbor)
		for(auto n = vNeighbor.begin(); n != vNeighbor.end(); n++)
		{
			if(MainGame::GetInstance()->GetButton(10 * n->y + n->x)->isBoom)
				count++;
		}
		resourceName = (RESOURCENAME)(BLOCK00 + count);

		m_bOpen = true;
		return true;
	}

	return false;
}
