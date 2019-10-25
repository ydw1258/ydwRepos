#include "GameFrameWork.h"
#include "ZFLog.h" //잠시 보류
ZFLog* g_pLog = new ZFLog(0, "TEST");// 몰라

HRESULT GameFrameWork::InitD3D(HWND hWnd)
{
	/*  //까만 Init
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
	
	cube.Init(g_pD3DDevice);
	cube.sv = { 2, 2, 2 };
	
	//텍스처
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	//카메라
	ZCamera::GetInstance()->SetupCamera(g_pD3DDevice);

	return S_OK;
	*/
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

	cube.Init(g_pD3DDevice);
	cube.sv = {10.0f, 10.0f, 10.0f };

	//카메라
	ZCamera::GetInstance()->SetupCamera(g_pD3DDevice);
	return S_OK;
}
void GameFrameWork::Update()
{
	cube.update();
	switch (scene)
	{
	case UNIVERSE:
		
		break;
	case CUBE_RENDER:
		
		break;
	case TRIANGLE:
		break;
	case TERRAIN:

		break;
	default:
		break;
	}
	Render();
}
/**-----------------------------------------------------------------------------
 * 광원 설정
 *------------------------------------------------------------------------------
 */
VOID GameFrameWork::SetupLights()
{
	/// 재질(material)설정
	/// 재질은 디바이스에 단 하나만 설정될 수 있다.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pD3DDevice->SetMaterial(&mtrl);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			/// 광원설정을 켠다

	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);		/// 환경광원(ambient light)의 값 설정
}
VOID GameFrameWork::Animate()
{
	//D3DXMatrixIdentity(&g_matAni);
	SetupLights();

	//D3DXMATRIXA16	m;
	//D3DXMATRIXA16	*pView;
	//pView = g_pCamera->GetViewMatrix();	// 카메라 클래스로부터 행렬정보를 얻는다.
	//m = *pView * g_matProj;				// World좌표를 얻기위해서 View * Proj행렬을 계산한다.
	//if (!g_bLockFrustum) g_pFrustum->Make(&m);	// View*Proj행렬로 Frustum을 만든다.

	LogFPS();
}
void GameFrameWork::Render()
{
	if (g_pD3DDevice == NULL)
		return;
	/// 후면버퍼와 Z버퍼 초기화
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, g_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

	/// 애니메이션 행렬설정
	Animate();
	cube.Render(g_pD3DDevice);
	//cube.Render(g_pD3DDevice);
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//여기에 그리기
		//tri.Render(g_pD3DDevice);
		
		//03_Matrices
		//돌아가는 삼각형
		//tri2.Render(g_pD3DDevice);

		switch (scene)
		{
		case UNIVERSE:
			break;
		case CUBE_RENDER:
			
			break;
		case TRIANGLE:
			break;
		case TERRAIN:

			break;
		default:
			break;
		}
		
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}



void GameFrameWork::CleanUp()
{
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}


/**-----------------------------------------------------------------------------
 * 입력 처리
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessInputs(HWND hwnd)
{
	ProcessMouse(hwnd);
	ProcessKey();
}

/**-----------------------------------------------------------------------------
 * 마우스 입력 처리
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessMouse(HWND g_hwnd)
{
	POINT	pt;
	float	fDelta = 0.001f;	// 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	GetCursorPos(&pt);
	int dx = pt.x - g_dwMouseX;	// 마우스의 변화값
	int dy = pt.y - g_dwMouseY;	// 마우스의 변화값

	
	ZCamera::GetInstance()->RotateLocalX(dy * fDelta);	// 마우스의 Y축 회전값은 3D world의  X축 회전값
	ZCamera::GetInstance()->RotateLocalY(dx * fDelta);	// 마우스의 X축 회전값은 3D world의  Y축 회전값
	D3DXMATRIXA16*	pmatView = ZCamera::GetInstance()->GetViewMatrix();		// 카메라 행렬을 얻는다.
	g_pD3DDevice->SetTransform(D3DTS_VIEW, pmatView);			// 카메라 행렬 셋팅

	// 마우스를 윈도우의 중앙으로 초기화
//	SetCursor( NULL );	// 마우스를 나타나지 않게 않다.
	RECT	rc;
	GetClientRect(g_hwnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_hwnd, &pt);
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
	
	if (GetAsyncKeyState('A')) ZCamera::GetInstance()->MoveLocalZ(0.5f);	// 카메라 전진!
	if (GetAsyncKeyState('Z')) ZCamera::GetInstance()->MoveLocalZ(-0.5f);	// 카메라 후진!
}
/**-----------------------------------------------------------------------------
 * FPS(Frame Per Second)출력
 *------------------------------------------------------------------------------
 */
void GameFrameWork::LogFPS(void)
{
	static DWORD	nTick = 0;
	static DWORD	nFPS = 0;

	/// 1초가 지났는가?
	if (GetTickCount() - nTick > 1000)
	{
		nTick = GetTickCount();
		/// FPS값 출력
		g_pLog->Log("FPS:%d", nFPS);

		nFPS = 0;
		LogStatus();	/// 상태정보를 여기서 출력(1초에 한번)
		return;
	}
	nFPS++;
}
void GameFrameWork::LogStatus(void)
{
	//g_pLog->Log("Wireframe:%d", g_bWireframe);
	//g_pLog->Log("HideFrustum:%d", g_bHideFrustum);
	//g_pLog->Log("LockFrustum:%d", g_bLockFrustum);
}
GameFrameWork::GameFrameWork(){}
GameFrameWork::~GameFrameWork(){}