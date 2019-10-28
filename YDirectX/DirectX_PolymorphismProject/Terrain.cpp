#include "Terrain.h"

HRESULT Terrain::InitTexture(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	if (FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice, BMP_HEIGHTMAP, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
		&g_pTexHeight)))
		return E_FAIL;
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, "tile2.tga", &g_pTexDiffuse)))
		return E_FAIL;

	return S_OK;
}

HRESULT Terrain::InitVB(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT	d3drc;

	g_pTexHeight->GetLevelDesc(0, &ddsd);
	g_cxHeight = ddsd.Width;
	g_czHeight = ddsd.Height;
	//ZFLog::GetInstance()->Log("Texturel Size:[%d,%d]", g_cxHeight, g_czHeight);
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(TERRAINVERTEX), 0,
		D3DFVF_CUSTOMVVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	g_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, g_cxHeight * g_czHeight * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
		return E_FAIL;

	TERRAINVERTEX v;
	TERRAINVERTEX* pV = (TERRAINVERTEX*)pVertices;

	for (DWORD z = 0; z < g_czHeight; z++)
	{
		for (DWORD x = 0; x < g_cxHeight; x++)
		{
			v.p.x = (float)x - g_cxHeight / 2.0f;
			v.p.z = -((float)z - g_czHeight / 2.0f);
			v.p.y = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x0000ff)) / 10.0f;
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize(&v.n, &v.n);
			v.t.x = (float)x / (g_cxHeight - 1);
			v.t.y = (float)z / (g_czHeight - 1);
			*pV++ = v;
			//g_pLog->Log("[%f ,%f ,%f]", v.p.x, v.p.y, v.p.z);
		}
	}

	g_pVB->Unlock();
	g_pTexHeight->UnlockRect(0);
	return S_OK;
}

HRESULT Terrain::InitIB(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	if (FAILED(g_pd3dDevice->CreateIndexBuffer((g_cxHeight - 1) * (g_czHeight - 1) * 2 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL)))
		return E_FAIL;

	MYINDEX		i;
	MYINDEX*	pI;
	if (FAILED(g_pIB->Lock(0, (g_cxHeight - 1) * (g_czHeight - 1) * 2 * sizeof(MYINDEX), (void**)&pI, 0)))
		return E_FAIL;

	for (DWORD z = 0; z < g_czHeight - 1; z++)
	{
		for (DWORD x = 0; x < g_cxHeight - 1; x++)
		{
			i._0 = (z*g_cxHeight + x);
			i._1 = (z*g_cxHeight + x + 1);
			i._2 = ((z + 1)*g_cxHeight + x);
			*pI++ = i;

			i._0 = ((z + 1)*g_cxHeight + x);
			i._1 = (z*g_cxHeight + x + 1);
			i._2 = ((z + 1)*g_cxHeight + x + 1);
			*pI++ = i;
		}
	}

	g_pIB->Unlock();
	return S_OK;
}
HRESULT Terrain::InitGeometry(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	D3DXMatrixIdentity(&g_matAni);

	if (FAILED(InitTexture(g_pd3dDevice)))
		return E_FAIL;

	if (FAILED(InitVB(g_pd3dDevice)))
		return E_FAIL;

	if (FAILED(InitIB(g_pd3dDevice)))
		return E_FAIL;

	return S_OK;
}
void Terrain::DrawMesh(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &finalmat);
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(TERRAINVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVVERTEX);
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_cxHeight * g_czHeight, 0, (g_cxHeight - 1) * (g_czHeight - 1) * 2);
}

void Terrain::Render(LPDIRECT3DDEVICE9& g_pd3dDevice)
{
	g_pd3dDevice->SetTexture(0, g_pTexDiffuse);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	D3DXMatrixScaling(&s, sv.x, sv.y, sv.z);
	D3DXMatrixTranslation(&t, tv.x, tv.y, tv.z);
	finalmat = s * t;

	DrawMesh(g_pd3dDevice);
}

void Terrain::CleanUp()
{
	SAFE_RELEASE(g_pTexHeight);
	SAFE_RELEASE(g_pTexDiffuse);
	SAFE_RELEASE(g_pIB);
	SAFE_RELEASE(g_pVB);
}

Terrain::Terrain(){}
Terrain::~Terrain(){}