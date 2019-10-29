#include "GameFrameWork.h"

void GameFrameWork::Init()
{
	//InitD3D(hWnd);
	cube.Init(g_pd3dDevice);
	ground.Init(g_pd3dDevice);
	SetupCamera();
	// 최초의 마우스 위치 보관
	POINT	pt;
	GetCursorPos(&pt);
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}
HRESULT GameFrameWork::InitD3D(HWND& _hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;
	hWnd = _hWnd;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}
void GameFrameWork::Update()
{
	cube.update();
	ProcessInputs();
	Render();
}

void GameFrameWork::SetupCamera()
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

	RECT rect;
	GetClientRect(hWnd, &rect);

	//투영
	D3DXMatrixPerspectiveLH(&matProj, D3DX_PI / 4, (float)rect.bottom / (float)rect.right, 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	ZCamera::GetInstance()->SetView(&vEyePt, &vLookatPt, &vUpVec);
}

void GameFrameWork::Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//Animate();
	SetupLights();
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		cube.Render(g_pd3dDevice);
		ground.Render();
		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

/**-----------------------------------------------------------------------------
 * FPS(Frame Per Second)출력
 *------------------------------------------------------------------------------
 */
VOID GameFrameWork::SetupLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	//vecDir = D3DXVECTOR3(1, 1, 1);
	vecDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f), 1.0f, sinf(GetTickCount() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;

	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00909090);
}

VOID GameFrameWork::Animate()
{
	/// 0 ~ 2PI 까지(0~360도) 값을 변화시킴 Fixed Point기법 사용
	DWORD d = GetTickCount() % ((int)((D3DX_PI * 2) * 1000));
	/// Y축 회전행렬
	//	D3DXMatrixRotationY( &g_matAni, d / 1000.0f );

	//LogFPS();

	SetupLights();
	ProcessInputs(); //hWnd
}

void GameFrameWork::CleanUp()
{
	ground.CleanUp();
	cube.Release();
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pD3D);
}

/**-----------------------------------------------------------------------------
 * 입력 처리
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessInputs()
{
	ProcessMouse();
	ProcessKey();
}

/**-----------------------------------------------------------------------------
 * 마우스 입력 처리
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessMouse()
{
	POINT	pt;
	float	fDelta = 0.001f;	// 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	GetCursorPos(&pt);
	int dx = pt.x - g_dwMouseX;	// 마우스의 변화값
	int dy = pt.y - g_dwMouseY;	// 마우스의 변화값


	ZCamera::GetInstance()->RotateLocalX(dy * fDelta);	// 마우스의 Y축 회전값은 3D world의  X축 회전값
	ZCamera::GetInstance()->RotateLocalY(dx * fDelta);	// 마우스의 X축 회전값은 3D world의  Y축 회전값
	D3DXMATRIXA16*	pmatView = ZCamera::GetInstance()->GetViewMatrix();		// 카메라 행렬을 얻는다.
	g_pd3dDevice->SetTransform(D3DTS_VIEW, pmatView);			// 카메라 행렬 셋팅

	// 마우스를 윈도우의 중앙으로 초기화
//	SetCursor( NULL );	// 마우스를 나타나지 않게 않다.
	RECT	rc;
	GetClientRect(hWnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}

/**-----------------------------------------------------------------------------
 * 키보드 입력 처리
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessKey(void)
{
	//if (GetAsyncKeyState('A')) ZCamera::GetInstance()->MoveLocalZ(0.5f);	// 카메라 전진!
	//if (GetAsyncKeyState('Z')) ZCamera::GetInstance()->MoveLocalZ(-0.5f);	// 카메라 후진!
/*
	if (GetAsyncKeyState('W'))
		
	if (GetAsyncKeyState('A'))
		
	if (GetAsyncKeyState('S'))

	if (GetAsyncKeyState('D'))
*/
}
GameFrameWork::GameFrameWork(){}
GameFrameWork::~GameFrameWork(){}


HRESULT GameFrameWork::InitGeometry()
{
	ground.Init(g_pd3dDevice);

	
	

	return S_OK;
}