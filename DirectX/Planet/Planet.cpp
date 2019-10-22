#include "Planet.h"

void Planet::Init(HWND hWnd)
{
	InitIB();
	InitVB();

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
		return;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice) < 0)
	{
		return;
	}

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}
void Planet::Render()
{
	if (g_pD3DDevice == NULL)
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{

		g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVECTEX));
		g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pD3DDevice->SetIndices(g_pIB);
		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void Planet::SetupMatrices()
{
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (1.0f * D3DX_PI) / 1000.0f;

	D3DXMatrixScaling(&matS, 1, 1, 1);
	D3DXMatrixRotationY(&matR, fAngle);
	D3DXMatrixTranslation(&matT, 0, 0, 0);

	matWorld = matS * matR * matT;
	//여기까지 로컬 스페이스
	//부모 SRT까지 계산하면 월드 스페이스

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//카메라 뷰스페이스
	D3DXVECTOR3 vEyept(0.0f, 3.0f, -20.0f);
	D3DXVECTOR3 vLootatPt(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyept, &vLootatPt, &vUpVec);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	//투영
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void Planet::Release()
{
	SAFE_RELEASE(g_pVB);
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

//로컬스페이스 VB IB
HRESULT Planet::InitVB()
{
	//정점들 선언
	CUSTOMVECTEX vertices[] =
	{
		{ -1 , 1 , 1 , 0xffff0000 } ,
		{ 1 , 1 , 1 , 0xff00ff00 } ,
		{ 1 ,   1 ,  -1 , 0xff0000ff },
		{ -1 , 1 ,  -1 , 0xffffff00 },

		{ -1 , -1 , 1 , 0xff00ffff } ,
		{ 1 , -1 , 1 , 0xffff00ff } ,
		{ 1 , -1 ,  -1 , 0xff000000 },
		{ -1 , -1 ,  -1 , 0xffffffff },
	};

	if (g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVECTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL) < 0)
		return E_FAIL;

	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}

HRESULT Planet::InitIB()
{
	//정점들로 이루고 있는 면 선언
	MYINDEX indeices[] =
	{
		{0,1,2},{0,2,3},//윗면
		{4,6,5},{4,7,6},//아랫면
		{0,3,7},{0,7,4},//왼면
		{1,5,6},{1,6,2},//오른면
		{3,2,6},{3,6,7},//앞면
		{0,4,5},{0,5,1}//뒷면
	};

	if (g_pD3DDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL) < 0)
	{
		return E_FAIL;
	}

	void* pIndices;
	if (g_pIB->Lock(0, sizeof(indeices), (void**)&pIndices, 0) < 0)
		return E_FAIL;
	memcpy(pIndices, indeices, sizeof(indeices));
	g_pIB->Unlock();

	return S_OK;
}


Planet::Planet()
{
}

Planet::~Planet()
{
}
