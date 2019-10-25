#include "GameFrameWork.h"
#include "ZFLog.h" //��� ����
ZFLog* g_pLog = new ZFLog(0, "TEST");// ����

HRESULT GameFrameWork::InitD3D(HWND hWnd)
{
	/*  //� Init
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
	
	//03_Matrices ���ư��� �ﰢ��
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//tri2.InitGeometry(g_pD3DDevice);
	
	cube.Init(g_pD3DDevice);
	cube.sv = { 2, 2, 2 };
	
	//�ؽ�ó
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
	//ī�޶�
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

	//ī�޶�
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
 * ���� ����
 *------------------------------------------------------------------------------
 */
VOID GameFrameWork::SetupLights()
{
	/// ����(material)����
	/// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pD3DDevice->SetMaterial(&mtrl);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			/// ���������� �Ҵ�

	g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);		/// ȯ�汤��(ambient light)�� �� ����
}
VOID GameFrameWork::Animate()
{
	//D3DXMatrixIdentity(&g_matAni);
	SetupLights();

	//D3DXMATRIXA16	m;
	//D3DXMATRIXA16	*pView;
	//pView = g_pCamera->GetViewMatrix();	// ī�޶� Ŭ�����κ��� ��������� ��´�.
	//m = *pView * g_matProj;				// World��ǥ�� ������ؼ� View * Proj����� ����Ѵ�.
	//if (!g_bLockFrustum) g_pFrustum->Make(&m);	// View*Proj��ķ� Frustum�� �����.

	LogFPS();
}
void GameFrameWork::Render()
{
	if (g_pD3DDevice == NULL)
		return;
	/// �ĸ���ۿ� Z���� �ʱ�ȭ
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, g_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

	/// �ִϸ��̼� ��ļ���
	Animate();
	cube.Render(g_pD3DDevice);
	//cube.Render(g_pD3DDevice);
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//���⿡ �׸���
		//tri.Render(g_pD3DDevice);
		
		//03_Matrices
		//���ư��� �ﰢ��
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
 * �Է� ó��
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessInputs(HWND hwnd)
{
	ProcessMouse(hwnd);
	ProcessKey();
}

/**-----------------------------------------------------------------------------
 * ���콺 �Է� ó��
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessMouse(HWND g_hwnd)
{
	POINT	pt;
	float	fDelta = 0.001f;	// ���콺�� �ΰ���, �� ���� Ŀ������ ���� �����δ�.

	GetCursorPos(&pt);
	int dx = pt.x - g_dwMouseX;	// ���콺�� ��ȭ��
	int dy = pt.y - g_dwMouseY;	// ���콺�� ��ȭ��

	
	ZCamera::GetInstance()->RotateLocalX(dy * fDelta);	// ���콺�� Y�� ȸ������ 3D world��  X�� ȸ����
	ZCamera::GetInstance()->RotateLocalY(dx * fDelta);	// ���콺�� X�� ȸ������ 3D world��  Y�� ȸ����
	D3DXMATRIXA16*	pmatView = ZCamera::GetInstance()->GetViewMatrix();		// ī�޶� ����� ��´�.
	g_pD3DDevice->SetTransform(D3DTS_VIEW, pmatView);			// ī�޶� ��� ����

	// ���콺�� �������� �߾����� �ʱ�ȭ
//	SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
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
 * Ű���� �Է� ó��
 *------------------------------------------------------------------------------
 */
void GameFrameWork::ProcessKey(void)
{
	
	if (GetAsyncKeyState('A')) ZCamera::GetInstance()->MoveLocalZ(0.5f);	// ī�޶� ����!
	if (GetAsyncKeyState('Z')) ZCamera::GetInstance()->MoveLocalZ(-0.5f);	// ī�޶� ����!
}
/**-----------------------------------------------------------------------------
 * FPS(Frame Per Second)���
 *------------------------------------------------------------------------------
 */
void GameFrameWork::LogFPS(void)
{
	static DWORD	nTick = 0;
	static DWORD	nFPS = 0;

	/// 1�ʰ� �����°�?
	if (GetTickCount() - nTick > 1000)
	{
		nTick = GetTickCount();
		/// FPS�� ���
		g_pLog->Log("FPS:%d", nFPS);

		nFPS = 0;
		LogStatus();	/// ���������� ���⼭ ���(1�ʿ� �ѹ�)
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