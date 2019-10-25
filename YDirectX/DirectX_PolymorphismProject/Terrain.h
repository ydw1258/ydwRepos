#pragma once
#include<d3d9.h>
#include<d3dx9.h>
#include "ZFLog.h"
#include "CUSTOM_DATATYPES.h"

#define WINDOW_W		500
#define WINDOW_H		500
#define WINDOW_TITLE	"HeightMap-TList"
#define BMP_HEIGHTMAP	"map128.bmp"

class Terrain
{
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
	LPDIRECT3DTEXTURE9			g_pTexHeight = NULL;
	LPDIRECT3DTEXTURE9			g_pTexDiffuse = NULL;
	D3DXMATRIXA16				g_matAni;
	DWORD						g_cxHeight = 0;
	DWORD						g_czHeight = 0;
	DWORD						g_dwMouseX = 0;
	DWORD						g_dwMouseY = 0;
public:
	Terrain();
	~Terrain();

	/*HRESULT InitTexture(LPDIRECT3DDEVICE9& g_pD3DDevice);
	HRESULT InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice);
	HRESULT InitIB(LPDIRECT3DDEVICE9& g_pD3DDevice);*/

};
