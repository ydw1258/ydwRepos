
#include <d3dx9.h>
#include <stdio.h>
#include "SAFE_DELETE.h"

#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")

#define WIN_WIDTH			800
#define WIN_HEIGHT			600

#define PI           3.14159265f
#define FOV          (PI/4.0f)							
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)		
#define NEAR_PLANE   1									
#define FAR_PLANE    10000								

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";

LPDIRECT3D9					g_pD3D = NULL;
LPDIRECT3DDEVICE9			g_pD3DDevice = NULL;
ID3DXFont*					g_pFont = NULL;
LPD3DXMESH					g_pTeapot = NULL;
LPD3DXEFFECT				g_pNormalMappingShader = NULL;
LPDIRECT3DTEXTURE9			g_pStoneDM = NULL;
LPDIRECT3DTEXTURE9			g_pStoneSM = NULL;
LPDIRECT3DTEXTURE9			g_pStoneNM = NULL;

float						g_RotationY = 0.0f;

D3DXVECTOR4					g_WorldLightPosition = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
D3DXVECTOR4					g_WorldCameraPos(0.0f, 0.0f, -200.0f, 1.0f);

D3DXVECTOR4					g_LightColor(0.7f, 0.7f, 1.0f, 1.0f);
D3DXVECTOR4					g_WorldCameraPosition(0.0f, 0.0f, -200.0f, 1.0f);

LPD3DXEFFECT LoadShader(const char* fileName);
LPD3DXMESH LoadModel(const char* fileName);
LPDIRECT3DTEXTURE9 LoadTexture(const char* fileName);

bool LoadAssets()
{
	//셰이더 로딩
	g_pNormalMappingShader = LoadShader("NormalMapping.fx");
	if (!g_pNormalMappingShader)
		return false;

	g_pTeapot = LoadModel("SphereWithTangent.x");
	if (!g_pTeapot)
		return false;

	g_pStoneDM = LoadTexture("Fieldstone_DM.tga");
	if (!g_pStoneDM)
		return false;

	g_pStoneSM = LoadTexture("fieldstone_SM.tga");
	if (!g_pStoneSM)
		return false;

	g_pStoneNM = LoadTexture("FieldStone_NM.tga");
	if (!g_pStoneNM)
		return false;

	return true;
}

HRESULT InitD3D(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferWidth = WIN_WIDTH;
	d3dpp.BackBufferHeight = WIN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		return E_FAIL;
	}

	//g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(D3DXCreateFont(g_pD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), "Arial", &g_pFont)))
	{
		return E_FAIL;
	}

	//Texture Load
	//Shader Load
	//Model Load
	if (!LoadAssets())
		return E_FAIL;


	return S_OK;
}

LPD3DXEFFECT LoadShader(const char* fileName)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlag = 0;

#if _DEBUG
	dwShaderFlag |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(g_pD3DDevice, fileName, NULL, NULL, dwShaderFlag, NULL, &ret, &pError);

	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

LPD3DXMESH LoadModel(const char* fileName)
{
	LPD3DXMESH ret = NULL;
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, g_pD3DDevice,
		NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패 : ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}

	return ret;
}

LPDIRECT3DTEXTURE9 LoadTexture(const char* fileName)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice, fileName, &ret)))
	{
		OutputDebugString("텍스쳐 로딩 실패 : ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}


void CleanUp()
{
	SAFE_RELEASE(g_pStoneDM);
	SAFE_RELEASE(g_pStoneSM);
	SAFE_RELEASE(g_pStoneNM);

	SAFE_RELEASE(g_pTeapot);
	SAFE_RELEASE(g_pNormalMappingShader);

	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

void RenderScene()
{
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vEyePt(g_WorldCameraPosition.x, g_WorldCameraPosition.y, g_WorldCameraPosition.z);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

	D3DXMATRIXA16			matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

	g_RotationY += 0.4f * PI / 180.0f;
	if (g_RotationY > 2 * PI)
	{
		g_RotationY -= 2 * PI;
	}

	D3DXMATRIXA16			matWorld;
	D3DXMatrixRotationY(&matWorld, g_RotationY);

	D3DXMATRIXA16 matWorldView;
	D3DXMATRIXA16 matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &matWorld, &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &matProjection);

	g_pNormalMappingShader->SetMatrix("gWorldMatrix", &matWorld);
	g_pNormalMappingShader->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);

	g_pNormalMappingShader->SetVector("gWorldLightPosition", &g_WorldLightPosition);
	g_pNormalMappingShader->SetVector("gWorldCameraPosition", &g_WorldCameraPosition);

	g_pNormalMappingShader->SetVector("gLightColor", &g_LightColor);
	g_pNormalMappingShader->SetTexture("DiffuseMap_Tex", g_pStoneDM);
	g_pNormalMappingShader->SetTexture("SpecularMap_Tex", g_pStoneSM);
	g_pNormalMappingShader->SetTexture("NormalMap_Tex", g_pStoneNM);

	UINT numPasses = 0;
	g_pNormalMappingShader->Begin(&numPasses, NULL);

	for (UINT i = 0; i < numPasses; i++)
	{
		g_pNormalMappingShader->BeginPass(i);

		g_pTeapot->DrawSubset(0);

		g_pNormalMappingShader->EndPass();
	}

	g_pNormalMappingShader->End();


}

void RenderInfo()
{
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	RECT rt = { 5,WIN_WIDTH / 3 , 5 , WIN_HEIGHT / 3 };
	g_pFont->DrawText(NULL, "데모 프래임워크\n\nESC : 데모종료", -1, &rt, 0, fontColor);

}

void Render()
{
	if (g_pD3DDevice == NULL)
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		RenderScene();
		RenderInfo();

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void Update()
{

}

void PlayDemo()
{
	Update();
	Render();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wc = { sizeof(WNDCLASSEX) , CS_CLASSDC , WndProc , 0 , 0 ,
		GetModuleHandle(NULL) , NULL , NULL , NULL , NULL , g_szClassName , NULL };
	RegisterClassEx(&wc);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	POINT ptDiff;
	RECT rcClient, rcWindow;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd, rcWindow.left, rcWindow.top, WIN_WIDTH + ptDiff.x, WIN_HEIGHT + ptDiff.y, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
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
				PlayDemo();
			}

		}
	}

	UnregisterClass(g_szClassName, wc.hInstance);

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

