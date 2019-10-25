#include "Cube.h"

Cube::Cube(){}
Cube::~Cube(){}

void Cube::Init(LPDIRECT3DDEVICE9 & g_pD3DDevice)
{
	InitVB(g_pD3DDevice);
	InitIB(g_pD3DDevice);
}
void Cube::update()
{
	Transform::Update();
}
void Cube::Render(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &(Transform::finalmat));
	g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVECTEX));
	g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pD3DDevice->SetIndices(g_pIB);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}
//Á¤Á¡À» Âï°í
HRESULT Cube::InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	CubeVERTEX vertices[] =
	{
		{ -1 , 1 , 1 , 0xffffffff } ,
		{ 1 , 1 , 1 , 0xffffffff } ,
		{ 1 ,   1 ,  -1 , 0xffffffff },
		{ -1 , 1 ,  -1 , 0xffffffff },

		{ -1 , -1 , 1 ,  0xffffffff} ,
		{ 1 , -1 , 1 ,  0xffffffff } ,
		{ 1 , -1 ,  -1 ,  0xffffffff },
		{ -1 , -1 ,  -1 ,  0xffffffff },
	};

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVECTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}
// Á¤Á¡ ±×¸±¼ø¼­ index Buffer
HRESULT Cube::InitIB(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	MYINDEX indeices[] =
	{
		{0,1,2},{0,2,3},//À­¸é
		{4,6,5},{4,7,6},//¾Æ·§¸é
		{0,3,7},{0,7,4},//¿Þ¸é
		{1,5,6},{1,6,2},//¿À¸¥¸é
		{3,2,6},{3,6,7},//¾Õ¸é
		{0,4,5},{0,5,1}//µÞ¸é
	};

	if (FAILED(g_pD3DDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
	{
		return E_FAIL;
	}


	void* pIndices;
	if (FAILED(g_pIB->Lock(0, sizeof(indeices), (void**)&pIndices, 0)))
		return E_FAIL;
	memcpy(pIndices, indeices, sizeof(indeices));
	g_pIB->Unlock();

	return S_OK;
}