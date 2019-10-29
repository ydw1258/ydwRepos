#include "Ground.h"

Ground::Ground()
{
}

Ground::~Ground()
{
}

HRESULT Ground::Init(LPDIRECT3DDEVICE9 _device)
{
	device = _device;

	D3DXMatrixIdentity(&mat);

	if (FAILED(InitTexture()))
		return E_FAIL;

	if (FAILED(InitVB()))
		return E_FAIL;

	if (FAILED(InitIB()))
		return E_FAIL;
}

void Ground::Render()
{
	device->SetTexture(0, texDiffuse);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&transMat, position.x, position.y, position.z);
	mat = scaleMat * transMat;

	DrawMesh(&mat);
}

void Ground::Release()
{
	CleanUp();
}

HRESULT Ground::InitTexture()
{
	if (FAILED(D3DXCreateTextureFromFileEx(device, BMP_HEIGHTMAP, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
		&texHeight)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(device, "tile2.tga", &texDiffuse)))
		return E_FAIL;

	return S_OK;
}

HRESULT Ground::InitVB()
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT	d3drc;

	texHeight->GetLevelDesc(0, &ddsd);
	g_cxHeight = ddsd.Width;
	g_czHeight = ddsd.Height;
	ZFLog::GetInstance()->Log("Texturel Size:[%d,%d]", g_cxHeight, g_czHeight);
	if (FAILED(device->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(TERRAINVERTEX), 0,
		D3DFVF_TEXTUREVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
		return E_FAIL;

	texHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
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
			vertVec.push_back(pV);
			//g_pLog->Log("[%f ,%f ,%f]", v.p.x, v.p.y, v.p.z);
		}
	}

	g_pVB->Unlock();
	texHeight->UnlockRect(0);
	return S_OK;
}

HRESULT Ground::InitIB()
{
	if (FAILED(device->CreateIndexBuffer((g_cxHeight - 1) * (g_czHeight - 1) * 2 * sizeof(MYINDEX), 0,
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

void Ground::CleanUp()
{
	SAFE_RELEASE(texHeight);
	SAFE_RELEASE(texDiffuse);
	SAFE_RELEASE(g_pIB);
	SAFE_RELEASE(g_pVB);
}

void Ground::DrawMesh(D3DXMATRIXA16 * pMat)
{
	device->SetTransform(D3DTS_WORLD, pMat);
	device->SetStreamSource(0, g_pVB, 0, sizeof(TERRAINVERTEX));
	device->SetFVF(D3DFVF_TEXTUREVERTEX);
	device->SetIndices(g_pIB);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, g_cxHeight * g_czHeight, 0,
		(g_cxHeight - 1) * (g_czHeight - 1) * 2);
}


