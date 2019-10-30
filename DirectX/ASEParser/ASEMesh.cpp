#include "ASEMesh.h"
#include "ASEParser.h"
#include "TextureManager.h"
#include "Animation.h"
#include "LabelRenderer.h"

ASEMesh::ASEMesh()
{
}


ASEMesh::~ASEMesh()
{
	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
		delete m_svBoneMesh[i];

	m_svBoneMesh.clear();

	for (size_t i = 0; i < m_svMesh.size(); i++)
		delete m_svMesh[i];

	m_svMesh.clear();
}


void ASEMesh::Init(LPDIRECT3DDEVICE9 pDevice, const char* fileName)
{
	ASEParser		cParser;
	vector< MeshData*> svMeshData;
	SMeterial* pMeterial = 0;

	cParser.Load(fileName, svMeshData, &pMeterial);

	if (svMeshData.empty())
		return;

	m_nVertexCount = 0;
	m_nFaceCount = 0;

	for (size_t i = 0; i < svMeshData.size(); i++)
	{
		Mesh* pMesh = new Mesh;
		DWORD	dwFVF = GetFVF(svMeshData[i]);
		pMesh->SetFVF(dwFVF);
		if (svMeshData[i]->nMeterialID >= 0)
		{
			pMesh->SetMaterial(pMeterial[svMeshData[i]->nMeterialID].dxMeterial);
			pMesh->SetTexture(TextureManager::GetInstance()->GetTexture(pDevice, pMeterial[svMeshData[i]->nMeterialID].strTexture.c_str()));
		}

		pMesh->SetName(svMeshData[i]->meshName);

		m_nVertexCount += svMeshData[i]->nVertexCount;
		m_nFaceCount += svMeshData[i]->nFaceCount;

		for (int j = 0; j < svMeshData[i]->nFaceCount; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				int		nIndex = svMeshData[i]->pFaceBuff[j].nIndex[k];
				pMesh->AddVertexBuffer(svMeshData[i]->pVertexBuff[nIndex]);

				if (dwFVF & D3DFVF_NORMAL)
					pMesh->AddNormalBuffer(svMeshData[i]->pFaceBuff[j].vNormal[k]);

				if (dwFVF & D3DFVF_TEX1)
					pMesh->AddUVBuffer(svMeshData[i]->pTexUVBuff[svMeshData[i]->pFaceBuff[j].nTIndex[k]]);

				if (dwFVF & D3DFVF_DIFFUSE)
					pMesh->AddColor(D3DXCOLOR(1, 0, 0, 1));

			}
		}

		for (int j = 0; j < svMeshData[i]->nFaceCount; j++)
		{
			pMesh->AddIndexBuffer(j * 3, j * 3 + 1, j * 3 + 2);
		}

		pMesh->Init(pDevice);
		pMesh->SetTM(svMeshData[i]->matTM);
		pMesh->SetInvTM(svMeshData[i]->matInvTM);

		if (strstr(svMeshData[i]->meshName.c_str(), "Bip") != 0 ||
			strstr(svMeshData[i]->meshName.c_str(), "Bone") != 0)
		{
			pMesh->SetBone(true);
			m_svBoneMesh.push_back(pMesh);

		}
		else
		{
			pMesh->SetBone(false);
			m_svMesh.push_back(pMesh);
		}
	}

	MakeHierarchy(svMeshData);

	//최초 로딩떄 무조건 애니메이션을 셋팅해주자

	if (Animation* pAnim = LoadAnim(svMeshData))
	{
		pAnim->SetFrameSpeed(cParser.m_nFrameSpeed);
		pAnim->SetFirsetFrame(cParser.m_nFirstFrame);
		pAnim->SetLastFrame(cParser.m_nLastFrame);
		pAnim->SetTickPerFrame(cParser.m_nTickPerFrame);
		m_pCurentAnim = pAnim;
	}

	for (size_t i = 0; i < m_svMesh.size(); i++)
	{
		m_svMesh[i]->SetAnimation(m_pCurentAnim);
	}

	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
	{
		m_svBoneMesh[i]->SetAnimation(m_pCurentAnim);
	}
}


Animation* ASEMesh::LoadAnim(vector<MeshData*>& svMeshData)
{
	Animation*		pAnimation = new Animation;
	bool	bAnimate = false;
	for (size_t i = 0; i < svMeshData.size(); i++)
	{
		for (size_t j = 0; j < svMeshData[i]->svRotAnim.size(); j++)
		{
			pAnimation->AddRotAnim(svMeshData[i]->meshName, svMeshData[i]->svRotAnim[j].nTime, svMeshData[i]->svRotAnim[j].qRotate);
			bAnimate = true;
		}

		for (size_t j = 0; j < svMeshData[i]->svPosAnim.size(); j++)
		{
			pAnimation->AddPosAnim(svMeshData[i]->meshName, svMeshData[i]->svPosAnim[j].nTime, svMeshData[i]->svPosAnim[j].vPosition);
			bAnimate = true;
		}
	}


	if (!bAnimate)
	{
		delete pAnimation;
		pAnimation = 0;
	}

	return pAnimation;
}


DWORD ASEMesh::GetFVF(MeshData* pMesh)
{
	if (pMesh == 0)
		return 0;

	DWORD	dwFVF = 0;

	if (pMesh->nVertexCount > 0)
		dwFVF |= D3DFVF_XYZ;

	if (pMesh->pFaceBuff != 0)
	{
		if (pMesh->pFaceBuff->vNormal[0] != D3DXVECTOR3(-1000, -1000, -1000))
			dwFVF |= D3DFVF_NORMAL;
	}

	if (pMesh->pTexUVBuff != 0)
		dwFVF |= D3DFVF_TEX1;
	else
		dwFVF |= D3DFVF_DIFFUSE;

	return dwFVF;
}


void ASEMesh::Render(LPDIRECT3DDEVICE9 pDevice)
{
	int		nTotalVertexCount = 0;

	LabelRenderer::GetInstance()->DrawLabel(10, 100, D3DXCOLOR(1, 0, 0, 1), "실제 버텍스 갯수 : %d", m_nVertexCount);
	LabelRenderer::GetInstance()->DrawLabel(10, 125, D3DXCOLOR(1, 0, 0, 1), "실제 페이스 갯수 : %d", m_nFaceCount);

	for (size_t i = 0; i < m_svMesh.size(); i++)
		nTotalVertexCount += m_svMesh[i]->GetVertexCount();

	LabelRenderer::GetInstance()->DrawLabel(10, 150, D3DXCOLOR(1, 0, 0, 1), "Mesh 랜더링 되는 버텍스 갯수 : %d", nTotalVertexCount);

	nTotalVertexCount = 0;
	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
		nTotalVertexCount += m_svBoneMesh[i]->GetVertexCount();

	LabelRenderer::GetInstance()->DrawLabel(10, 175, D3DXCOLOR(1, 0, 0, 1), "Bone 랜더링 되는 버텍스 갯수 : %d", nTotalVertexCount);

	for (size_t i = 0; i < m_svMesh.size(); i++)
	{
		if (m_svMesh[i]->GetParent() == 0)
			m_svMesh[i]->DrawMesh(pDevice);
	}

	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
	{
		if (m_svBoneMesh[i]->GetParent() == 0)
			m_svBoneMesh[i]->DrawMesh(pDevice);
	}
}


void ASEMesh::MakeHierarchy(vector< MeshData*>& svMeshData)
{
	for (size_t i = 0; i < m_svMesh.size(); i++)
	{
		MeshData* pMeshData = GetMeshData(svMeshData, m_svMesh[i]->GetName());

		if (pMeshData->bParent)
		{
			Mesh* pParent = FindNode(m_svMesh, pMeshData->parentName);

			if (pParent != 0)
			{
				m_svMesh[i]->SetParent(pParent);
				pParent->AddChild(m_svMesh[i]);
			}
			else
			{
				pParent = FindNode(m_svBoneMesh, pMeshData->parentName);
				m_svMesh[i]->SetParent(pParent);
				pParent->AddChild(m_svMesh[i]);
			}
		}
	}

	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
	{
		MeshData* pMeshData = GetMeshData(svMeshData, m_svBoneMesh[i]->GetName());

		if (pMeshData->bParent)
		{
			Mesh* pParent = FindNode(m_svBoneMesh, pMeshData->parentName);

			if (pParent)
			{
				m_svBoneMesh[i]->SetParent(pParent);
				pParent->AddChild(m_svBoneMesh[i]);
			}
		}
	}
}


MeshData*	ASEMesh::GetMeshData(vector< MeshData*>& svMeshData, string strNode)
{
	for (size_t i = 0; i < svMeshData.size(); i++)
	{
		if (svMeshData[i]->meshName.compare(strNode) == 0)
			return svMeshData[i];
	}

	return 0;
}


Mesh* ASEMesh::FindNode(vector<Mesh*>& svNode, string strNode)
{
	for (size_t i = 0; i < svNode.size(); i++)
	{
		if (svNode[i]->GetName() == strNode)
		{
			return svNode[i];
		}
	}

	return 0;
}


void ASEMesh::Update(D3DXMATRIXA16& matTM, float fEllipseTime)
{
	static float fFrame = 0.f;

	fFrame += fEllipseTime * m_pCurentAnim->GetAnimSpeed();


	if (fFrame > m_pCurentAnim->GetAnimLastFrameTick())
		fFrame = m_pCurentAnim->GetAnimFirstFrameTick();

	for (size_t i = 0; i < m_svBoneMesh.size(); i++)
	{
		if (m_svBoneMesh[i]->GetParent() == 0)
			m_svBoneMesh[i]->Update(matTM, fFrame);
	}

	for (size_t i = 0; i < m_svMesh.size(); i++)
	{
		if (m_svMesh[i]->GetParent() == 0)
			m_svMesh[i]->Update(matTM, fFrame);
	}
}