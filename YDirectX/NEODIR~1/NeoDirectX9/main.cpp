#include <d3d9.h>
#include <SAFE_DELETE.h>

#include <vector>

#pragma comment (lib, "d3d9")
#pragma comment (lib, "d3dx9")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitVectexBuffer();
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";

LPDIRECT3D9			g_pD3D = nullptr;
LPDIRECT3DDEVICE9	g_pD3DDevice = nullptr;


struct  CUSTOMVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

std::vector< LPDIRECT3DVERTEXBUFFER9> g_VectorVertexBuffer;
std::vector< CUSTOMVERTEX*> g_VectorCustom;

#define BUFFERVECSIZE 2


void InitVectexBuffer()
{
	for (int i = 0; i < BUFFERVECSIZE; i++)
	{
		LPDIRECT3DVERTEXBUFFER9 Tmp = nullptr;
		g_VectorVertexBuffer.push_back(Tmp);
	}
}


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


HRESULT InitD3D(HWND hwnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == nullptr)
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		return E_FAIL;
	}

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT  InitVB()
{
	for (int i = 0; i < BUFFERVECSIZE / 2; i++)
	{
		CUSTOMVERTEX vertices1[3] =
		{
			{150.0f + i * 100.0f, 50.0f, 0.5f, 1.0f, 0xffff0000},
			{250.0f + i * 100.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00},
			{50.0f + i * 100.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff}
		};

		CUSTOMVERTEX vertices2[3] =
		{
			{150.0f+ i * 100.0f, 50.0f, 0.5f, 1.0f, 0xffff0000},
			{350.0f+ i * 100.0f, 50.0f, 0.5f, 1.0f, 0xff00ff00},
			{250.0f+ i * 100.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff}
		};

		g_VectorCustom.push_back(vertices1);
		g_VectorCustom.push_back(vertices2);
	}

	for (int i = 0; i < BUFFERVECSIZE; i++)
	{
		if (FAILED(g_pD3DDevice->CreateVertexBuffer(
			3 * sizeof(CUSTOMVERTEX),
			0,
			D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&g_VectorVertexBuffer[i],
			nullptr)))
		{
			return E_FAIL;
		}
	}

	// 메모리에 값을 채우기 위해 락을 채운다. ㅇㅇㅇ
	void* pVertices;
	   
	for (int i = 0; i < BUFFERVECSIZE; i++)
	{
		if (FAILED(g_VectorVertexBuffer[i]->Lock(0, sizeof((*g_VectorCustom[i])) * 3, (void**)&pVertices, 0)))
		{
			return E_FAIL;
		}

		memcpy(pVertices, g_VectorCustom[i], sizeof((*g_VectorCustom[i])) * 3);
		g_VectorVertexBuffer[i]->Unlock();
	}

	return S_OK;
}

void cleanUp()
{
	for (size_t i = 0; i < g_VectorVertexBuffer.size(); i++)
	{
		SAFE_RELEASE(g_VectorVertexBuffer[i]);
	}
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

void Render()
{
	if (g_pD3DDevice == nullptr)
	{
		return;
	}

	g_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0
		, 255), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		for (int i = 0; i < BUFFERVECSIZE; i++)
		{
			g_pD3DDevice->SetStreamSource(0, g_VectorVertexBuffer[i], 0, sizeof(CUSTOMVERTEX));
			g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
			g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		}

		// 옵션에 따라 버텍스가 다르게 그려질 수 있다.

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

int APIENTRY WinMain(HINSTANCE hInsatance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS  WndClass;
	g_hInst = hInsatance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInsatance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInsatance, NULL);

	InitVectexBuffer();

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
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
				{
					Render();
				}

			}
		}
	}

	cleanUp();
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

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}