#pragma once
#include <d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

struct CUSTOMVECTEX
{
	float x, y, z;
	DWORD color;
};
struct MYINDEX
{
	WORD _0, _1, _2;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
class Planet
{
private:
	LPDIRECT3DDEVICE9 g_pD3DDevice;
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matS;
	D3DXMATRIXA16 matR;
	D3DXMATRIXA16 matT;
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
	LPDIRECT3D9					g_pD3D = NULL;
public:
	void Init(HWND hWnd);
	void Render();
	HRESULT InitIB();
	HRESULT InitVB();
	Planet();
	~Planet();
	void SetupMatrices();
	void Update();
	void DrawMesh();
	void Release();
};