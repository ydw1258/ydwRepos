#include <d3dx9.h>
#include <mmsystem.h>

//#include "SAFE_DELETE.h"
#include "SolarManager.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

#include "Mercury.h"
#include "Venus.h"
#include "Earth.h"
#include "Mars.h"
#include "Sun.h"
#include "Moon.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND g_hWnd;

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pd3dDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL;

D3DXMATRIXA16	g_matTMParent;
D3DXMATRIXA16	g_matRParent;

D3DXMATRIXA16	g_matTMChild;
D3DXMATRIXA16	g_matRChild;

float g_fRot = .0f;

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct  MYINDEX
{
	WORD _0, _1, _2;
};

D3DXVECTOR3 g_aniPos[2];
D3DXQUATERNION g_aniRot[2];

HRESULT InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] =
	{
		{-1, 1, 1, 0xffff0000},
		{  1, 1, 1, 0xff00ff00 },
		{  1, 1, -1, 0xff0000ff },
		{ -1, 1, -1, 0xffffff00 },

		{ -1, -1, 1, 0xff00ffff },
		{ 1, -1, 1, 0xffff00ff },
		{ 1, -1, -1, 0xff000000 },
		{ -1, -1, -1, 0xffffffff },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}

HRESULT InitIB()
{
	MYINDEX indices[] =
	{
		{ 0,1,2 },{ 0,2,3 },
		{ 4,6,5 },{ 4,7,6 },
		{ 0,3,7 },{ 0,7,4 },
		{ 1,5,6 },{ 1,6,2 },
		{ 3,2,6 },{ 3,6,7 },
		{ 0,4,5 },{ 0,5,1 },
	};

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
		&g_pIB, NULL)))
		return E_FAIL;

	VOID* pIndeces;
	if (FAILED(g_pIB->Lock(0, sizeof(indices), (void**)&pIndeces, 0)))
		return E_FAIL;
	memcpy(pIndeces, indices, sizeof(indices));
	g_pIB->Unlock();

	return S_OK;
}

void InitAnimation()
{
	g_aniPos[0] = D3DXVECTOR3(0, 0, 0);
	g_aniPos[1] = D3DXVECTOR3(5, 5, 5);

	FLOAT Yaw = D3DX_PI * 90.0f / 180.0f;
	FLOAT Pitch = .0f;
	FLOAT Roll = .0f;

	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[0], Yaw, Pitch, Roll);

	Yaw = .0f;
	Pitch = D3DX_PI * 90.0f / 180.0f;
	Roll = .0f;

	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[1], Yaw, Pitch, Roll);
}


HRESULT InitGeometry()
{
	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;

	InitAnimation();

	return S_OK;
}

void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 50.0f, -50.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


float Linear(float v0, float v1, float t)
{
	return v0 * (1.0f - t) + v1 * t;
}

VOID Animate()
{
	static float t = .0f;
	float x, y, z;
	D3DXQUATERNION quat;

	/*if (t > 1.0f)
	{
		t = .0f;
	}*/

	x = Linear(g_aniPos[0].x, g_aniPos[1].x, t);
	y = Linear(g_aniPos[0].y, g_aniPos[1].y, t);
	z = Linear(g_aniPos[0].z, g_aniPos[1].z, t);

	D3DXMatrixTranslation(&g_matTMParent, 0.0f, .0f, .0f);

	// 사원수 - 내부 개념
	D3DXQuaternionSlerp(&quat, &g_aniRot[0], &g_aniRot[1], t);
	D3DXMatrixRotationQuaternion(&g_matRParent, &quat);
	t += 0.005f;
	D3DXMatrixRotationZ(&g_matRChild, GetTickCount() / 500.0f);
	D3DXMatrixTranslation(&g_matTMChild, 3, 3, 3);
}

VOID Cleanup()
{
	/*SAFE_RELEASE(g_pIB);
	SAFE_RELEASE(g_pVB);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pD3D);*/
}



void DrawMesh(D3DXMATRIXA16* pMat)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}



VOID Render()
{
	D3DXMATRIXA16 matWorld;

	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	Animate();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		/*matWorld = g_matRParent * g_matTMParent;
		DrawMesh(&matWorld);

		matWorld = g_matRChild * g_matTMChild * matWorld;
		DrawMesh(&matWorld);

		matWorld = g_matRChild * g_matTMChild * matWorld;
		DrawMesh(&matWorld);

*/
		DrawMesh(&SolarManager::inst()->pSun->MyFinalMat);
		DrawMesh(&SolarManager::inst()->pMercury->MyFinalMat);
		DrawMesh(&SolarManager::inst()->pVenus->MyFinalMat);
		DrawMesh(&SolarManager::inst()->pEarth->MyFinalMat);
		DrawMesh(&SolarManager::inst()->pMoon->MyFinalMat);
		DrawMesh(&SolarManager::inst()->pMars->MyFinalMat);

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


char g_szClassName[256] = "BasicFrame";
#define ROT_DELTA 0.1f

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	switch (iMessage)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
			g_fRot -= ROT_DELTA;
		if (wParam == VK_RIGHT)
			g_fRot += ROT_DELTA;
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	if (FAILED(InitD3D(hWnd)))
		return -1;

	if (FAILED(InitGeometry()))
		return -1;

	SetupCamera();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SolarManager::inst()->init();

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			SolarManager::inst()->update();
			Render();
		}
	}

	UnregisterClass(g_szClassName, hInstance);

	return (int)Message.wParam;
}