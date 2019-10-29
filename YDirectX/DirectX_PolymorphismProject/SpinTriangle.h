#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#define D3DFVF_TEXTUREVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

//Ãâ·Â¾ÈµÊ
class SpinTriangle
{
private:
	LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
public:
	SpinTriangle();
	~SpinTriangle();

	HRESULT InitGeometry(LPDIRECT3DDEVICE9& g_pD3DDevice);
	void SetupMareices(PDIRECT3DDEVICE9& g_pD3DDevice);
	void Render(LPDIRECT3DDEVICE9& g_pD3DDevice);
};

