#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include <map>
#include "MeshDataEtc.h"
using namespace std;

class Mesh;
class Animation
{
private:
	map<string, vector<SRotAnimation*>	>	m_smRotAnim;
	map<string, vector<SPosAnimation*>	>	m_smPosAnim;

	int										m_nFrameSpeed;
	int										m_nTickPerFrame;
	int										m_nFirstFrame;
	int										m_nLastFrame;

	SRotAnimation*	GetRotTrack(string strNode, float fFrame);
	SPosAnimation*  GetPosTrack(string strNode, float fFrame);
	SPosAnimation*  GetNextPosTrack(string strNode, SPosAnimation* pPos);
	SRotAnimation*  GetNextRotTrack(string strNode, SRotAnimation* pPos);
	void	ReleaseAnimte();
public:

	Animation();
	~Animation();

	void			AddRotAnim(string strNode, int nTime, D3DXQUATERNION	qRotate);
	void			AddPosAnim(string strNode, int nTime, D3DXVECTOR3		vPosition);
	D3DXMATRIXA16	Animate(Mesh* pMesh, float fFrame);
	void			SetFrameSpeed(int nSpeed) { m_nFrameSpeed = nSpeed; }
	void			SetFirsetFrame(int nFrame) { m_nFirstFrame = nFrame; }
	void			SetLastFrame(int nFrame) { m_nLastFrame = nFrame; }
	void			SetTickPerFrame(int nTickPerFrame) { m_nTickPerFrame = nTickPerFrame; }

	float			GetAnimSpeed() { return (float)(m_nTickPerFrame * m_nFrameSpeed); }
	float			GetTatalAniTime() { return (float)(m_nTickPerFrame * m_nLastFrame); }
	float			GetAnimFirstFrameTick() { return (float)(m_nFirstFrame * m_nTickPerFrame); }
	float			GetAnimLastFrameTick() { return (float)(m_nLastFrame * m_nTickPerFrame); }


	bool operator ()(SRotAnimation* lhs, SRotAnimation* rhs) const
	{
		return lhs->nTime < rhs->nTime ? true : false;
	}

	bool operator ()(SPosAnimation* lhs, SPosAnimation* rhs) const
	{
		return lhs->nTime < rhs->nTime ? true : false;
	}
};

