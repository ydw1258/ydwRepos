#include <d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";

LPDIRECT3D9					g_pD3D = NULL;
LPDIRECT3DDEVICE9			g_pD3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVB = NULL;
LPDIRECT3DINDEXBUFFER9		g_pIB = NULL;


struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct MYINDEX
{
	WORD _0, _1, _2;
};



HRESULT InitD3D(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		return E_FAIL;
	}

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}

HRESULT InitVB()
{
	CUSTOMVERTEX vertices[] = 
	{
		{-1, 1, 1, 0xffff0000},
		{1, 1, 1, 0xff00ff00},
		{1, 1, -1, 0xff0000ff},
		{-1, 1, -1, 0xffffff00},

		{-1, -1, 1, 0xff00ffff},
		{1, -1, 1, 0xffff00ff},
		{1, -1, -1, 0xff000000},
		{-1, -1, -1, 0xffffffff},
	};

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(8* sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}


	void* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}

HRESULT InitIB()
{
	MYINDEX indeices[] =
	{
		{0,1,2},{0,2,3},
		{4,5,6},{4,7,6},
		{0,3,7},{0,7,4},
		{1,5,6},{1,6,2},
		{3,2,6},{3,6,7},
		{0,4,5},{0,5,1}
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


void CleanUp()
{
	SAFE_RELEASE(g_pVB);
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

void SetupMareices()
{
	D3DXMATRIXA16 matWorld;
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fAngle);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//wasd

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

void Render()
{
	if (g_pD3DDevice == NULL)
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		SetupMareices();
		g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pD3DDevice->SetIndices(g_pIB);

		// 인덱스 버퍼가 깡으로 버텍스 박는 거 보다 배로 빠르다
		g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

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


	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			if (SUCCEEDED(InitIB()))
			{
				ShowWindow(hWnd, nCmdShow);
				UpdateWindow(hWnd);

				ZeroMemory(&Message, sizeof(Message));
				while (Message.message != WM_QUIT)
				{
					if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
					{
						TranslateMessage(&Message);
						DispatchMessage(&Message);
					}
					else
						Render();
				}
			}
		}

	}

	CleanUp();

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}



// 기하벡터 - 크기 + 방향 가진 수량, 위치값은 중요치 않음
// 위치 벡터 3D 상의 위치를 나타냄
// 스칼라: 크기만 존재하는 수치(방향성 X)

// dot product
// 벡터의 내적: x * y, (x1 * y1) + (x2 * y2) + (x3 * y3) -> 결과값 - 스칼라
// |a||b|cos Theta
// arc(a * b / |a||b|) Theta
// 내적시 1 이면 평행 - 0이면 수직 cos 때문에


// cross product
// 벡터의 외적: x X y, 
//		(x2 * y3 - x3 * y2), 
//		(x1 * y3 - x3 * y1), 
//		(x1 * y2 - x2 * y1)
// 결과값 - 벡터
// (|a||b|sin)u -> u -> a와 b의 수직인 단위벡터

// 교환, 결합법칙 성립
// a + b = b + a
// (a + b) + c = a + (b + c)

// 법선 벡터(normal vector) -> 음영 제거 시
// 2D -> 특정 [벡터]에 수직인 벡터
// 3D -> 특정 [평면]에 수직인 벡터
// 음면 제거에 사용 -> cross를 사용해 거꾸로 된거 판단



// 행렬 - 교환법칙이 성립하지 않음, 결합법칙은 성립
// 역행렬 - 모든 행렬이 역행렬이 존재하지 않음