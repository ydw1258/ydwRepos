#pragma once
#include <d3d9.h>
#include<d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Transform.h"



#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

class Cube : public Transform
{
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
	D3DXVECTOR3 g_aniPos[2];
	D3DXQUATERNION g_aniRot[2];
public:
	Cube();
	~Cube();

	void Init(LPDIRECT3DDEVICE9& g_pD3DDevice);
	void update();
	void Render(LPDIRECT3DDEVICE9& g_pD3DDevice);

	void InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice);
	void InitIB(LPDIRECT3DDEVICE9& g_pD3DDevice);
	void InitAnimation();
	void Release();
};

