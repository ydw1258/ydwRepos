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

	// FVF값을 이용한 Mesh생성
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
		MessageBox(NULL, "LockAttributeBuffer FAILED", "알림", MB_OK);
	}

	memset(pData, 0, sizeof(DWORD) * m_svIndex.size() / 3);	//★

	pSysMesh->UnlockAttributeBuffer();

	LPD3DXBUFFER pAdjacencyBuffer = NULL;
	if (FAILED(res = D3DXCreateBuffer(m_svIndex.size() * sizeof(DWORD), &pAdjacencyBuffer)))
	{
		MessageBox(NULL, "D3DXCreateBuffer FAILED", "알림", MB_OK);
		return;
	}

	//인덱스 버터 기준으로 인접성 정보를 추출
	if (FAILED(res = pSysMesh->GenerateAdjacency(0.0f, (DWORD*)pAdjacencyBuffer->GetBufferPointer())))
	{
		MessageBox(NULL, "GenerateAdjacency FAILED", "알림", MB_OK);
		return;
	}


	////-------------------------------------------------------------------------	
	//// H/W 버퍼 최적화 
	////-------------------------------------------------------------------------	
	//// pSysMesh 를 최적화,  m_pMesh 에 최적화된 새 인터페이스를 돌려준다.★
	OptimizeMesh(pDevice , pSysMesh, pAdjacencyBuffer, &m_dxMesh);

	SAFE_RELEASE(pAdjacencyBuffer);
	

	if(pSysMesh)
		pSysMesh->Release();
}



void Mesh::OptimizeMesh(LPDIRECT3DDEVICE9 pDevice , LPD3DXMESH pSysMesh, LPD3DXBUFFER pAdjacencyBuffer, LPD3DXMESH* ppOptimizedMesh)
{
	HRESULT res = D3D_OK;

	LPD3DXMESH pOptMesh = NULL;	//최적화가 끝난 메쉬 인터페이스 포인터.

	D3DXWELDEPSILONS Eps;
	memset(&Eps, 0, sizeof(D3DXWELDEPSILONS));
	Eps.Position = 0.001f;				//위치와 텍스쳐 좌표를 가지고 weld 한다.
										//Eps.Normal = 0.5f;				 
										//Eps.Texcoord[0] = 0.01f;

										// 새롭게 노멀버퍼를 만들어주자
	D3DXComputeNormals(pSysMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer());

	// 이건 얼만큼 섬세하게 weight에대한 단계 처리다

	if (FAILED(res = D3DXWeldVertices(pSysMesh,
		//D3DXWELDEPSILONS_WELDALL,			 
		//D3DXWELDEPSILONS_DONOTSPLIT,
		//D3DXWELDEPSILONS_WELDPARTIALMATCHES,	 
		D3DXWELDEPSILONS_WELDPARTIALMATCHES | D3DXWELDEPSILONS_DONOTREMOVEVERTICES,
		&Eps,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL)))
	{
		MessageBox(NULL, "D3DXWeldVertices FAILED", "알림", MB_OK);
		//return;
	}

	// 인접 face 정보를 사용해서 정점 정보를 최적화 한다.

	// 해당 매쉬를 Attribute단위로 소팅을하여 최적화를 할 수 있다. 즉,
	// 같은 메터리얼을 사용하는 삼각형들을 인접하게 하여 최적화를 하는 것이다.

	if (FAILED(res = pSysMesh->Optimize(
		//D3DXMESHOPT_ATTRSORT | D3DXMESH_MANAGED, 
		//D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE ,
		D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE | D3DXMESH_SYSTEMMEM,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL, &pOptMesh)))
	{
		MessageBox(NULL, "pSysMesh->Optimize FAILED", "알림", MB_OK);
		//return;
	}

	//-------------------------------------------------------------------------	
	//  Getting a Optimized Buffer  
	//-------------------------------------------------------------------------	
	//
	// pSysMesh 최적화후 생성된 pOptMesh 로 부터 최종 출력에 사용될 인터페이스를 
	// ppOptimizeMesh 에 넘겨준다. 
	// 이때,  ppOptimizeMesh 인터페이스는 데이터를 수정 할 필요가  없기 때문에 
	// 조금 더 빠른 속도를 위해 WriteOnly 로 설정한다.
	//
	if (FAILED(res = pOptMesh->CloneMeshFVF(D3DXMESH_WRITEONLY | D3DXMESH_MANAGED,
		pOptMesh->GetFVF(), pDevice, ppOptimizedMesh)))
	{

	}

	//임시 버퍼 삭제.
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