#pragma once
#include"BitMap.h"
#include<vector>
using namespace std;

class MineButton
{
private:
	BitMap*		m_pBitMap;
	BitMap*		m_pBackBitMap;
	int			m_iX;
	int			m_iY;
	bool		m_bOpen;
	RESOURCENAME resourceName;
	vector<POINT> vNeighbor;
public:
	bool isBoom;
	void Init(int x, int y);
	void Draw(HDC hdc);
	bool Input(POINT pt);

	MineButton();
	~MineButton();
};