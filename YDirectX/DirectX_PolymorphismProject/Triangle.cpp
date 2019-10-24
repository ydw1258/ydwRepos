#include "Triangle.h"

Triangle::Triangle(){}
Triangle::~Triangle(){}

HRESULT Triangle::InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	CUSTOMVECTEX vertices[] =
	{
		{ 150.0f , 50.0f , 0.5f , 1.0f , 0xffff0000} ,
		{ 250.0f , 250.0f , 0.5f , 1.0f , 0xff00ff00 } ,
		{ 50.0f , 250.0f , 0.5f , 1.0f , 0xff00ffff },
	};

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVECTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	//돌아가는 삼각형
	g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVECTEX));
	g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	return S_OK;
}



void Triangle::Render(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVECTEX));
	g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}
