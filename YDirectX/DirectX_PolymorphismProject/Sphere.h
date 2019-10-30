#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Transform.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

class Sphere
{
private:
	Transform transform;
	LPD3DXMESH					g_pSphere = NULL;
	LPD3DXEFFECT				g_pColorShader = NULL;
	LPD3DXEFFECT				g_pTextureShader = NULL;
	LPDIRECT3DTEXTURE9			g_pEarthDM = NULL;
public:
	Sphere();
	~Sphere();
	bool Init(LPDIRECT3DDEVICE9& g_pD3DDevice);
	LPD3DXEFFECT LoadShader(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice);
	LPD3DXMESH LoadModel(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice);
	LPDIRECT3DTEXTURE9 LoadTexture(const char* fileName, LPDIRECT3DDEVICE9& g_pD3DDevice);
	void Render();
};