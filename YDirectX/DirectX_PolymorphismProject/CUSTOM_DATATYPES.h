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
	D3DXVECTOR3 position; // The transformed position for the vertex.
	DWORD color;        // The vertex color.
};
struct MYINDEX
{
	WORD _0, _1, _2;
};
//FVF 옵션
#define D3DFVF_TEXTUREVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

#define D3DFVF_CUBEVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUBEVERTEX2 (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_NORMAL)
//D3DFVF_DIFFUSE 컬러
//D3DFVF_NORMAL 빛 연산에 필요
//D3DFVF_TEX1 텍스처 옵션 (컬러랑 동시에 사용잘 안함)

struct TERRAINVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR3	t;
};