#include "SpinTriangle.h"

SpinTriangle::SpinTriangle(){}
SpinTriangle::~SpinTriangle(){}

HRESULT SpinTriangle::InitGeometry(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	CUSTOMVECTEX2 vertices[] =
	{
		{ -1.0f , -1.0f , 0.0f , 0xffff0000 } ,
		{ 1.0f , -1.0f , 0.0f , 0xff0000ff } ,
		{ 0.0f , 1.0f ,  0.0f , 0xffffffff }
	};

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVECTEX2), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}
void SpinTriangle::SetupMareices(PDIRECT3DDEVICE9& g_pD3DDevice)
{
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	D3DXMATRIXA16 matWorld;
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	//D3DXMatrixIdentity(&matWorld2);
	//D3DXMatrixTranslation(&matWorld , )
	//D3DXMatrixScaling()
	D3DXMatrixRotationY(&matWorld, fAngle);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyept(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLootatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyept, &vLootatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}
void SpinTriangle::Render(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	SetupMareices(g_pD3DDevice);

	g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVECTEX2));
	g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}