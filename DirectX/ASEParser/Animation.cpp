#include "Animation.h"
#include <algorithm>
#include "Mesh.h"

Animation::Animation()
{
	m_smRotAnim.clear();
	m_smPosAnim.clear();
}


Animation::~Animation()
{
}


void Animation::AddRotAnim(string strNode, int nTime, D3DXQUATERNION	qRotate)
{
	SRotAnimation* 	sRot = new SRotAnimation;
	sRot->nTime = nTime;
	sRot->qRotate = qRotate;

	map<string, vector<SRotAnimation*>	>::iterator itor = m_smRotAnim.find(strNode);

	if (itor != m_smRotAnim.end())
	{
		itor->second.push_back(sRot);
		sort(itor->second.begin(), itor->second.end(), Animation());
	}
	else
	{
		vector<SRotAnimation*>		svList;
		svList.push_back(sRot);
		m_smRotAnim.insert(make_pair(strNode, svList));
	}

}

void Animation::AddPosAnim(string strNode, int nTime, D3DXVECTOR3		vPosition)
{
	SPosAnimation*	sPos = new SPosAnimation;
	sPos->nTime = nTime;
	sPos->vPosition = vPosition;

	map<string, vector<SPosAnimation*>	>::iterator itor = m_smPosAnim.find(strNode);

	if (itor != m_smPosAnim.end())
	{
		itor->second.push_back(sPos);
		sort(itor->second.begin(), itor->second.end(), Animation());
	}
	else
	{
		vector<SPosAnimation*>		svList;
		svList.push_back(sPos);
		m_smPosAnim.insert(make_pair(strNode, svList));
	}
}

SRotAnimation* Animation::GetRotTrack(string strNode, float fFrame)
{
	map<string, vector<SRotAnimation*>	>::iterator itor = m_smRotAnim.find(strNode);

	if (itor == m_smRotAnim.end())
		return 0;

	vector< SRotAnimation* > svList = itor->second;

	if (svList.size() == 0 || svList[0]->nTime > fFrame)
		return 0;

	SRotAnimation* pRef = 0;
	float	fOldFrame = 0.f;

	for (unsigned int i = 0; i < svList.size(); i++)
	{
		SRotAnimation* pTemp = svList[i];
		float	fCurFrame = (float)pTemp->nTime;

		if (pRef == 0)
		{
			fOldFrame = fCurFrame;	pRef = pTemp;
		}
		else if (fabs(fOldFrame - fFrame) > fabs(fCurFrame - fFrame) && fCurFrame <= fFrame)
		{
			fOldFrame = fCurFrame;	pRef = pTemp;
		}
	}

	return pRef;
}

SPosAnimation* Animation::GetPosTrack(string strNode, float fFrame)
{
	map<string, vector<SPosAnimation*>	>::iterator itor = m_smPosAnim.find(strNode);

	if (itor == m_smPosAnim.end())
		return 0;

	vector< SPosAnimation* > svList = itor->second;

	SPosAnimation* pRef = 0;
	float	fOldFrame = 0.f;

	for (unsigned int i = 0; i < svList.size(); i++)
	{
		SPosAnimation* pTemp = svList[i];
		float	fCurFrame = (float)pTemp->nTime;

		if (pRef == 0)
		{
			fOldFrame = fCurFrame;	pRef = pTemp;
		}
		else if (fabs(fOldFrame - fFrame) > fabs(fCurFrame - fFrame) && fCurFrame <= fFrame)
		{
			fOldFrame = fCurFrame;	pRef = pTemp;
		}
	}

	return pRef;
}

SPosAnimation* Animation::GetNextPosTrack(string strNode, SPosAnimation* pPos)
{
	map<string, vector<SPosAnimation*>	>::iterator itor = m_smPosAnim.find(strNode);

	if (itor == m_smPosAnim.end())
		return 0;

	vector< SPosAnimation* > svList = itor->second;

	for (size_t i = 0; i < svList.size() - 1; i++)
	{
		if (svList[i] == pPos)
			return svList[i + 1];
	}

	return 0;
}

SRotAnimation* Animation::GetNextRotTrack(string strNode, SRotAnimation* pPos)
{
	map<string, vector<SRotAnimation*>	>::iterator itor = m_smRotAnim.find(strNode);

	if (itor == m_smRotAnim.end())
		return 0;

	vector< SRotAnimation* > svList = itor->second;

	for (size_t i = 0; i < svList.size() - 1; i++)
	{
		if (svList[i] == pPos)
			return svList[i + 1];
	}

	return 0;
}

D3DXMATRIXA16	Animation::Animate(Mesh* pMesh, float fFrame)
{
	D3DXMATRIXA16		matTM;
	D3DXMATRIXA16		matRot;

	D3DXMatrixIdentity(&matTM);
	D3DXMatrixIdentity(&matRot);

	SRotAnimation* pRot = GetRotTrack(pMesh->GetName(), fFrame);

	D3DXQUATERNION qRot;
	D3DXQUATERNION qRotNext;

	if (pRot != 0)
	{
		qRot = pRot->qRotate;
		SRotAnimation* pNext = GetNextRotTrack(pMesh->GetName(), pRot);

		if (pNext)
		{
			qRotNext = pNext->qRotate;
			float fPercent = (fFrame - (float)pRot->nTime) / (float)(pNext->nTime - pRot->nTime);
			D3DXQuaternionSlerp(&qRot, &qRot, &qRotNext, fPercent);
		}

		D3DXMatrixRotationQuaternion(&matRot, &qRot);
	}
	else
	{
		if (pMesh->GetParent())
		{
			matRot = pMesh->GetTM() * pMesh->GetParent()->GetInvTM();
		}
		else
		{
			matRot = pMesh->GetTM();
		}
	}

	matRot._41 = matRot._42 = matRot._43 = 0.f;	matRot._44 = 1.f;


	D3DXMATRIXA16		matTran;

	D3DXVECTOR3			vPos;
	D3DXVECTOR3			vNextPos;

	SPosAnimation* pPos = GetPosTrack(pMesh->GetName(), fFrame);

	if (pPos != 0)
	{
		vPos = pPos->vPosition;
		SPosAnimation* pNext = GetNextPosTrack(pMesh->GetName(), pPos);

		if (pNext != 0)
		{
			vNextPos = pNext->vPosition;
			float fPercent = (fFrame - (float)pPos->nTime) / (float)(pNext->nTime - pPos->nTime);
			D3DXVec3Lerp(&vPos, &vPos, &vNextPos, fPercent);
		}
	}
	else
	{
		if (pMesh->GetParent())
		{
			matTran = pMesh->GetTM() * pMesh->GetParent()->GetInvTM();

		}
		else
		{
			matTran = pMesh->GetTM();
		}

		vPos.x = matTran._41;		vPos.y = matTran._42;	vPos.z = matTran._43;
	}

	matTM = matRot;
	matTM._41 = vPos.x; matTM._42 = vPos.y; matTM._43 = vPos.z;

	return matTM;
}


void Animation::ReleaseAnimte()
{
	map<string, vector<SRotAnimation*>>::iterator itor = m_smRotAnim.begin();

	while (itor != m_smRotAnim.end())
	{
		for (size_t i = 0; i < itor->second.size(); i++)
		{
			delete itor->second[i];
		}

		itor->second.clear();
		itor++;
	}

	map<string, vector<SPosAnimation*>>::iterator itor2 = m_smPosAnim.begin();

	while (itor2 != m_smPosAnim.end())
	{
		for (size_t i = 0; i < itor2->second.size(); i++)
		{
			delete itor2->second[i];
		}

		itor2->second.clear();
		itor2++;
	}

}