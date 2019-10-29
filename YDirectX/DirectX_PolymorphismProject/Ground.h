#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "ZFLog.h"
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include <vector>
using namespace std;

#define WINDOW_TITLE	"HeightMap-TList"
#define BMP_HEIGHTMAP	"map128.bmp"

class Ground
{
private:
	LPDIRECT3DDEVICE9 device;
	LPDIRECT3DTEXTURE9 texHeight = NULL;
	LPDIRECT3DTEXTURE9 texDiffuse = NULL;
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;

	D3DXMATRIXA16 mat;
	D3DXMATRIXA16 scaleMat;
	D3DXMATRIXA16 transMat;
	D3DXVECTOR3 position = { 0,0,0 };
	D3DXVECTOR3 scale = { 1,1,1 };

	DWORD g_cxHeight = 0;
	DWORD g_czHeight = 0;

	vector<TERRAINVERTEX*> vertVec;

public:
	TERRAINVERTEX* getVert(int x, int z) {
		x += g_cxHeight / 2;
		z *= -1;
		z += g_czHeight / 2;

		if (z*g_cxHeight + x < 0 || z * g_cxHeight + x >= vertVec.size())
			return nullptr;

		return vertVec[z*g_cxHeight + x];
	}

public:
	Ground();
	~Ground();

	HRESULT Init(LPDIRECT3DDEVICE9 _device);
	void Render();
	void Release();

	HRESULT InitTexture();
	HRESULT InitVB();
	HRESULT InitIB();

	void CleanUp();
	void DrawMesh(D3DXMATRIXA16* pMat);
};

