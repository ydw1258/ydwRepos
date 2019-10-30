#include "Mesh.h"
#include "Animation.h"
#include "SAFE_DELETE.h"
#include "GameManager.h"

Mesh::Mesh()
{
	m_dxMesh = 0;
	m_pTexture = 0;
	ZeroMemory(&m_dxMaterial, sizeof(D3DMATERIAL9));
	m_dwFVF = 0;
	m_pParent = 0;
	m_svChild.clear();
	m_bBone = false;

	m_pAnimation = 0;
	D3DXMatrixIdentity(&m_matTM);
	D3DXMatrixInverse(&m_matInvTM, 0, &m_matTM);
	D3DXMatrixIdentity(&m_matCurTM);
}


Mesh::~Mesh()
{
}


void Mesh::Init(LPDIRECT3DDEVICE9 device)
{
	HRESULT res = D3D_OK;

	LPD3DXMESH pSysMesh = 0;

	// FVF���� �̿��� Mesh����
	if (FAILED(res = D3DXCreateMeshFVF(m_svIndex.size() / 3, m_svVertex.size(),
		D3DXMESH_SYSTEMMEM,
		m_dwFVF, device, &pSysMesh)))
	{
		return;
	}

	if (!m_svVertex.empty())		MakeVertexBuffer(pSysMesh);
	if (!m_svIndex.empty())		MakeIndexBuffer(pSysMesh);

	CreateAttributeBuffer(device , pSysMesh);
}

void Mesh::MakeVertexBuffer(LPD3DXMESH pSysMesh)
{
	if (pSysMesh == 0)
		return;

	void* pData = 0;
	pSysMesh->LockVertexBuffer(0, &pData);

	if (pData != 0)
	{
		for (size_t i = 0; i < m_svVertex.size(); i++)
		{
			if (m_dwFVF & D3DFVF_XYZ)
			{
				memcpy(pData, &m_svVertex[i], sizeof(D3DXVECTOR3));
				pData = (D3DXVECTOR3*)pData + 1;
			}
			else if (m_dwFVF & D3DFVF_XYZRHW)
			{
				D3DXVECTOR4 temp(m_svVertex[i].x, m_svVertex[i].y, m_svVertex[i].z, 1);
				memcpy(pData, &temp, sizeof(D3DXVECTOR4));
				pData = (D3DXVECTOR4*)pData + 1;
			}

			if (m_dwFVF & D3DFVF_DIFFUSE)
			{
				DWORD	dwColor = 0xff00ff00;
				if (i < m_svColor.size())
					dwColor = m_svColor[i];

				memcpy(pData, &dwColor, sizeof(DWORD));
				pData = (DWORD*)pData + 1;
			}

			if (m_dwFVF & D3DFVF_NORMAL)
			{
				memcpy(pData, &m_svNormal[i], sizeof(D3DXVECTOR3));
				pData = (D3DXVECTOR3*)pData + 1;
			}

			if (m_dwFVF & D3DFVF_TEX1)
			{
				memcpy(pData, &m_svUVBuffer[i], sizeof(D3DXVECTOR2));
				pData = (D3DXVECTOR2*)pData + 1;
			}
		}
	}

	pSysMesh->UnlockVertexBuffer();
}


void Mesh::MakeIndexBuffer(LPD3DXMESH pSysMesh)
{
	if (pSysMesh == 0)
		return;

	void* pData = 0;
	pSysMesh->LockIndexBuffer(0, &pData);

	memset(pData, 0, sizeof(WORD) * m_svIndex.size());

	if (m_svIndex.size() != 0)
		memcpy(pData, &m_svIndex[0], sizeof(WORD) * m_svIndex.size());


	pSysMesh->UnlockIndexBuffer();
}


void Mesh::DrawMesh(LPDIRECT3DDEVICE9 pDevice)
{
	DWORD		dwLight = 0;
	pDevice->GetRenderState(D3DRS_LIGHTING, &dwLight);

	if (m_dwFVF & D3DFVF_DIFFUSE)
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	else
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetMaterial(&m_dxMaterial);
	pDevice->SetTexture(0, m_pTexture);

	pDevice->SetTransform(D3DTS_WORLD, &m_matCurTM);

	if (IsVisible())
	{
		if (m_dxMesh)
			m_dxMesh->DrawSubset(0);
	}

	if (m_svChild.size() > 0)
	{
		for (auto i : m_svChild)
		{
			i->DrawMesh(pDevice);
		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, dwLight);
}

int	Mesh::GetFVFSize()
{
	int		nSize = 0;

	if (m_dwFVF & D3DFVF_XYZ)
		nSize += sizeof(D3DXVECTOR3);
	else if (m_dwFVF & D3DFVF_XYZRHW)
		nSize += (sizeof(float) * 4);

	if (m_dwFVF & D3DFVF_NORMAL)
		nSize += sizeof(D3DXVECTOR3);

	if (m_dwFVF & D3DFVF_DIFFUSE)
		nSize += sizeof(DWORD);

	if (m_dwFVF & D3DFVF_TEX1)
		nSize += sizeof(D3DXVECTOR2);

	return nSize;
}


void Mesh::CreateAttributeBuffer(LPDIRECT3DDEVICE9 pDevice , LPD3DXMESH pSysMesh)
{
	HRESULT res = D3D_OK;

	if (pSysMesh == 0)
		return;

	DWORD* pData = 0;
	if (FAILED(res = pSysMesh->LockAttributeBuffer(0, &pData)))
	{
		MessageBox(NULL, "LockAttributeBuffer FAILED", "�˸�", MB_OK);
	}

	memset(pData, 0, sizeof(DWORD) * m_svIndex.size() / 3);	//��

	pSysMesh->UnlockAttributeBuffer();

	LPD3DXBUFFER pAdjacencyBuffer = NULL;
	if (FAILED(res = D3DXCreateBuffer(m_svIndex.size() * sizeof(DWORD), &pAdjacencyBuffer)))
	{
		MessageBox(NULL, "D3DXCreateBuffer FAILED", "�˸�", MB_OK);
		return;
	}

	//�ε��� ���� �������� ������ ������ ����
	if (FAILED(res = pSysMesh->GenerateAdjacency(0.0f, (DWORD*)pAdjacencyBuffer->GetBufferPointer())))
	{
		MessageBox(NULL, "GenerateAdjacency FAILED", "�˸�", MB_OK);
		return;
	}


	////-------------------------------------------------------------------------	
	//// H/W ���� ����ȭ 
	////-------------------------------------------------------------------------	
	//// pSysMesh �� ����ȭ,  m_pMesh �� ����ȭ�� �� �������̽��� �����ش�.��
	OptimizeMesh(pDevice , pSysMesh, pAdjacencyBuffer, &m_dxMesh);

	SAFE_RELEASE(pAdjacencyBuffer);
	

	if(pSysMesh)
		pSysMesh->Release();
}



void Mesh::OptimizeMesh(LPDIRECT3DDEVICE9 pDevice , LPD3DXMESH pSysMesh, LPD3DXBUFFER pAdjacencyBuffer, LPD3DXMESH* ppOptimizedMesh)
{
	HRESULT res = D3D_OK;

	LPD3DXMESH pOptMesh = NULL;	//����ȭ�� ���� �޽� �������̽� ������.

	D3DXWELDEPSILONS Eps;
	memset(&Eps, 0, sizeof(D3DXWELDEPSILONS));
	Eps.Position = 0.001f;				//��ġ�� �ؽ��� ��ǥ�� ������ weld �Ѵ�.
										//Eps.Normal = 0.5f;				 
										//Eps.Texcoord[0] = 0.01f;

										// ���Ӱ� ��ֹ��۸� ���������
	D3DXComputeNormals(pSysMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer());

	// �̰� ��ŭ �����ϰ� weight������ �ܰ� ó����

	if (FAILED(res = D3DXWeldVertices(pSysMesh,
		//D3DXWELDEPSILONS_WELDALL,			 
		//D3DXWELDEPSILONS_DONOTSPLIT,
		//D3DXWELDEPSILONS_WELDPARTIALMATCHES,	 
		D3DXWELDEPSILONS_WELDPARTIALMATCHES | D3DXWELDEPSILONS_DONOTREMOVEVERTICES,
		&Eps,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL)))
	{
		MessageBox(NULL, "D3DXWeldVertices FAILED", "�˸�", MB_OK);
		//return;
	}

	// ���� face ������ ����ؼ� ���� ������ ����ȭ �Ѵ�.

	// �ش� �Ž��� Attribute������ �������Ͽ� ����ȭ�� �� �� �ִ�. ��,
	// ���� ���͸����� ����ϴ� �ﰢ������ �����ϰ� �Ͽ� ����ȭ�� �ϴ� ���̴�.

	if (FAILED(res = pSysMesh->Optimize(
		//D3DXMESHOPT_ATTRSORT | D3DXMESH_MANAGED, 
		//D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE ,
		D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE | D3DXMESH_SYSTEMMEM,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL, &pOptMesh)))
	{
		MessageBox(NULL, "pSysMesh->Optimize FAILED", "�˸�", MB_OK);
		//return;
	}

	//-------------------------------------------------------------------------	
	//  Getting a Optimized Buffer  
	//-------------------------------------------------------------------------	
	//
	// pSysMesh ����ȭ�� ������ pOptMesh �� ���� ���� ��¿� ���� �������̽��� 
	// ppOptimizeMesh �� �Ѱ��ش�. 
	// �̶�,  ppOptimizeMesh �������̽��� �����͸� ���� �� �ʿ䰡  ���� ������ 
	// ���� �� ���� �ӵ��� ���� WriteOnly �� �����Ѵ�.
	//
	if (FAILED(res = pOptMesh->CloneMeshFVF(D3DXMESH_WRITEONLY | D3DXMESH_MANAGED,
		pOptMesh->GetFVF(), pDevice, ppOptimizedMesh)))
	{

	}

	//�ӽ� ���� ����.
	if (pOptMesh)
		pOptMesh->Release();
}


void Mesh::Update(D3DXMATRIXA16& matTM, float fFrame)
{
	D3DXMatrixIdentity(&m_matCurTM);
	D3DXMATRIXA16	matAnim = m_pAnimation != 0 ? m_pAnimation->Animate(this, fFrame) : m_matTM;

	m_matCurTM = matAnim  * matTM;

	if (m_svChild.empty())
		return;

	for (auto i : m_svChild)
	{
		i->Update(m_matCurTM, fFrame);
	}
}

void Mesh::Release()
{
	SAFE_RELEASE(m_dxMesh);
}

bool Mesh::IsVisible()
{
	bool bVisible = true;

	if (!GameManager::GetInstance()->m_bBoneView && m_bBone)
		bVisible = false;

	if (!GameManager::GetInstance()->m_bMeshView && !m_bBone)
		bVisible = false;

	return bVisible;
}