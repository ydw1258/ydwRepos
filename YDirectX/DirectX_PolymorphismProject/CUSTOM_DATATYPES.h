#pragma once
#include <d3d9.h>

struct CUSTOMVECTEX
{
	float x, y, z, rhw;
	DWORD color;
};

struct CUSTOMVECTEX2
{
	float x, y, z;
	DWORD color;
};

struct CUSTOMVECTEX
{
	D3DXVECTOR3 position;
	D3DCOLOR color;
#ifndef SHOW_HOW_TO_USE_TCI
	float	tu, tv;
#endif
};
