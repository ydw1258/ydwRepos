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
struct CubeVERTEX
{
	float x, y, z;
	DWORD color;
};
struct MYINDEX
{
	WORD _0, _1, _2;
};
/*
struct TERRAINVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR3	t;
};
*/