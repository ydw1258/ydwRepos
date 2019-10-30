#include "GameManager.h"
#include "Timer.h"
#include "ASEMesh.h"
#include "Camera.h"
#include "InputManager.h"
#include "LabelRenderer.h"
#include "SAFE_DELETE.h"

GameManager* GameManager::m_pThis = NULL;

GameManager::GameManager()
{
	m_bWireFrame = false;
	m_bBoneView = false;
	m_bMeshView = true;
}


GameManager::~GameManager()
{
}

HRESULT GameManager::Init(HWND hWnd , HINSTANCE hInst)
{
	if (!(m_pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	if (CreateDevice(hWnd) == E_FAIL)
		return E_FAIL;

	InitLight();

	m_pTimer = new Timer();
	m_pTimer->Init();

	m_pCamera = new Camera();
	m_pCamera->Init(m_pDevice , D3DXVECTOR3(0, 10, -20), D3DXVECTOR3(0, 0, 0), 60.f, WINSIZE_X / WINSIZE_Y, 1.f, 1000.f);

	m_pMesh = new ASEMesh();
	m_pMesh->Init(m_pDevice , "Data/woman_01_all.ase");

	InputManager::GetInstance()->Init(hInst, hWnd);
	LabelRenderer::GetInstance()->Init(m_pDevice);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void GameManager::Update()
{
	if (InputManager::GetInstance()->OnceKeyDown(DIK_SPACE))
		m_bWireFrame = !m_bWireFrame;

	if (InputManager::GetInstance()->OnceKeyDown(DIK_F5))
		m_bBoneView = !m_bBoneView;

	if (InputManager::GetInstance()->OnceKeyDown(DIK_F6))
		m_bMeshView = !m_bMeshView;

	InputManager::GetInstance()->Update();
	m_pTimer->Update();

	m_pCamera->Update(m_pDevice, m_pTimer->GetElapsedTime());

	D3DXMATRIXA16		mat;
	D3DXMatrixIdentity(&mat);

	/*static float	fRot = 0.f;
	fRot += D3DX_PI / 4 * m_cTimer->GetElapsedTime();
	D3DXMatrixRotationX( &mat, fRot );*/

	m_pMesh->Update(mat, m_pTimer->GetElapsedTime());

	Render();
}


void GameManager::Render()
{
	if (m_pDevice == 0)
		return;

	if (m_bWireFrame)
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	DWORD	dwColor = m_bWireFrame ? D3DXCOLOR(0.6f, 0.6f, 0.6f, 1) : D3DXCOLOR(0, 0.3f, 0.8f, 1.f);
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, dwColor, 1.0f, 0);

	///< ������ ���� ȭ���� �׸���.
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		LabelRenderer::GetInstance()->DrawLabel("Space : ���̾��� On / Off", 10, 300, D3DXCOLOR(1, 0, 0, 1));
		LabelRenderer::GetInstance()->DrawLabel("F5 : Bone On / Off", 10, 325, D3DXCOLOR(1, 0, 0, 1));
		LabelRenderer::GetInstance()->DrawLabel("F6 : Mesh On / Off", 10, 350, D3DXCOLOR(1, 0, 0, 1));

		if (m_pMesh)
			m_pMesh->Render(m_pDevice);
		
		m_pCamera->Render();

		m_pDevice->EndScene();
	}

	m_pDevice->Present(0, 0, 0, 0);
}


void GameManager::Release()
{

	SAFE_DELETE(m_pTimer);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pMesh);

	LabelRenderer::GetInstance()->Release();
	InputManager::GetInstance()->Release();
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3d);

}

void GameManager::InitLight()
{
	D3DLIGHT9		light;
	memset(&light, 0, sizeof(D3DLIGHT9));

	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;
	light.Diffuse.a = 1.f;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(0, 0, 1);

	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, true);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);

}

HRESULT	GameManager::CreateDevice(HWND hWnd)
{
	D3DCAPS9					sCaps;
	if (FAILED(m_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &sCaps)))
	{
		return E_FAIL;
	}

	int			vp = 0;
	D3DDEVTYPE	eType;

	if (sCaps.VertexProcessingCaps != 0)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		///<  PURE ����̽� �˻�
		if (sCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
		{
			vp |= D3DCREATE_PUREDEVICE;
		}

		eType = D3DDEVTYPE_HAL;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		eType = D3DDEVTYPE_SW;
	}

	D3DFORMAT dxBackFormat = D3DFMT_A8R8G8B8;

	int		nBackWidth = WINSIZE_X;
	int		nBackHeight = WINSIZE_Y;


	///< ����̽� �Ӽ��� ä���ش�.
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.hDeviceWindow = hWnd;					///< ������ �ڵ�
	d3dpp.Windowed = TRUE;						///< ������ ��弳�� 
	d3dpp.BackBufferCount = 1;						///< ����� ����
	d3dpp.BackBufferWidth = nBackWidth;					///< ���� 
	d3dpp.BackBufferHeight = nBackHeight;					///< ����
	d3dpp.BackBufferFormat = dxBackFormat;				///< �ĸ������ �ȼ� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	///< �ø���(����Ʈ���ۿ� ����۸� �ٲ��ִ°�)ü���� ���۰� ��ȯ�Ǵ� ����� �����ϴ� D3DSWAPEFFECT������ ���
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		///< ��Ƽ����
	d3dpp.MultiSampleQuality = 0;						///< ��Ƽ����
	d3dpp.EnableAutoDepthStencil = TRUE;						///< ????
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			// �⺻


	if (FAILED(m_pD3d->CreateDevice(sCaps.AdapterOrdinal, sCaps.DeviceType, hWnd, vp, &d3dpp, &m_pDevice)))
	{
		///< ����̽� ���� ���� ó�� 
		return E_FAIL;
	}


	return S_OK;
}