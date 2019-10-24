#include "GameFrameWork.h"

HRESULT GameFrameWork::InitD3D(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		return E_FAIL;
	}
	//tri.InitVB(g_pD3DDevice);
	
	//03_Matrices 돌아가는 삼각형
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//tri2.InitGeometry(g_pD3DDevice);
	
	//텍스처
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

void GameFrameWork::Render()
{
	if (g_pD3DDevice == NULL)
		return;

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//여기에 그리기
		//tri.Render(g_pD3DDevice);
		
		//03_Matrices
		//돌아가는 삼각형
		//tri2.Render(g_pD3DDevice);

		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
void GameFrameWork::CleanUp()
{
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}
GameFrameWork::GameFrameWork()
{
}


GameFrameWork::~GameFrameWork()
{
}
