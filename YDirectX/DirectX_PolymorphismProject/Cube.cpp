#include "Cube.h"

Cube::Cube(){}
Cube::~Cube(){}

void Cube::Init(LPDIRECT3DDEVICE9 & g_pD3DDevice)
{
	InitVB(g_pD3DDevice);
	InitIB(g_pD3DDevice);
}
void Cube::update()
{
	Transform::TransformUpdate();
}
void Cube::Render(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &finalmat);
	g_pD3DDevice->SetStreamSource(0, g_pVB, 0, sizeof(CubeVERTEX));
	g_pD3DDevice->SetIndices(g_pIB);
	g_pD3DDevice->SetFVF(D3DFVF_CUBEVERTEX); //FVF옵션 VB초기화, 렌더시 필요
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 6 * 2);
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//정점을 찍고
void Cube::InitVB(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	CubeVERTEX Vertices[] = {
		{ D3DXVECTOR3(-1.0f, 1.0f,  1.0f) ,  D3DCOLOR_RGBA(255, 0, 255, 255) },
		{ D3DXVECTOR3(1.0f, 1.0f,  1.0f)  , D3DCOLOR_RGBA(0, 255, 0, 255) },
		{ D3DXVECTOR3(1.0f, 1.0f, -1.0f)  , D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, 1.0f, -1.0f)  , D3DCOLOR_RGBA(255, 255, 0, 255) },
		{ D3DXVECTOR3(-1.0f, -1.0f,  1.0f) , D3DCOLOR_RGBA(255, 0, 0, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f,  1.0f) , D3DCOLOR_RGBA(0, 255, 10, 255) },
		{ D3DXVECTOR3(1.0f, -1.0f, -1.0f) , D3DCOLOR_RGBA(0, 0, 255, 255) },
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f) , D3DCOLOR_RGBA(255, 0, 0, 255) }
	};
	//FVF옵션
	g_pD3DDevice->CreateVertexBuffer(8 * sizeof(CubeVERTEX), 0, D3DFVF_CUBEVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL);

	void* pVertices;
	g_pVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0);

	memcpy(pVertices, Vertices, sizeof(Vertices));
	g_pVB->Unlock();

}
// 정점 그릴순서 index Buffer
void Cube::InitIB(LPDIRECT3DDEVICE9& g_pD3DDevice)
{
	MYINDEX indeices[] =
	{
		{0,1,2},{0,2,3},//윗면
		{4,6,5},{4,7,6},//아랫면
		{0,3,7},{0,7,4},//왼면
		{1,5,6},{1,6,2},//오른면
		{3,2,6},{3,6,7},//앞면
		{0,4,5},{0,5,1}//뒷면
	};

	g_pD3DDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL);

	void* pIndices;
	g_pIB->Lock(0, sizeof(indeices), (void**)&pIndices, 0);
	memcpy(pIndices, indeices, sizeof(indeices));
	g_pIB->Unlock();
}
void Cube::InitAnimation()
{
	g_aniPos[0] = D3DXVECTOR3(0, 0, 0);
	g_aniPos[1] = D3DXVECTOR3(5, 5, 5);

	FLOAT Yaw = D3DX_PI * 90.0f / 180.0f;
	FLOAT Pitch = 0;
	FLOAT Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[0], Yaw, Pitch, Roll);

	Yaw = 0;
	Pitch = D3DX_PI * 90 / 180.0f;
	Roll = 0;

	D3DXQuaternionRotationYawPitchRoll(&g_aniRot[1], Yaw, Pitch, Roll);
}

void Cube::Release()
{
	if (g_pVB != NULL)
		g_pVB->Release();
	if (g_pIB != NULL)
		g_pIB->Release();
}
