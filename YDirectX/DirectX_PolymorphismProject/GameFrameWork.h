#pragma once
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Triangle.h"
#include "SpinTriangle.h"
#include "Cube.h"
#include "ZCamera.h"


//dx �׷��� ī�忡 ���
//a �׸� �ɼ� a draw b �׸� �ɼ� b draw

enum Scene
{
	UNIVERSE,
	CUBE_RENDER,
	TRIANGLE,
	TERRAIN
};
class GameFrameWork
{
private:
	LPDIRECT3D9			g_pD3D = NULL;
	LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;
	
	Scene scene = UNIVERSE;

	Triangle tri;
	SpinTriangle tri2;
	Cube cube;

	
	
public:
	HRESULT InitD3D(HWND hWnd);
	
	void CleanUp();
	void Update();
	void Render();

	DWORD g_dwMouseX = 0;
	DWORD g_dwMouseY = 0;
	void ProcessInputs(HWND hwnd);
	void ProcessMouse(HWND g_hwnd);
	void ProcessKey();

	//�������� �ø�
	BOOL					g_bWireframe = FALSE;	// ���̾����������� �׸����ΰ�?
	BOOL					g_bHideFrustum = TRUE;	// Frustum�� �ȱ׸� ���ΰ�?
	BOOL					g_bLockFrustum = FALSE;	// Frustum�� ������ ���ΰ�?
	//HRESULT ProcessFrustumCull();

	VOID Animate();
	VOID SetupLights();
	void LogFPS(void);
	void LogStatus(void);

	GameFrameWork();
	~GameFrameWork();
	
};