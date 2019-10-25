#pragma once
#include <d3d9.h>
#include<d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Transform.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

class Cube : public Transform
{
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;
public:
	Cube();
	~Cube();

	void Init(LPDIRECT3DDEVICE9& g_pD3DDevice);
	void update();
	void Render(LPDIRECT3DDEVICE9& g_pD3DDevice);
	HRESULT InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice);
	HRESULT InitIB(LPDIRECT3DDEVICE9& g_pD3DDevice);

};

