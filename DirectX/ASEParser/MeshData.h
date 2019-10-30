#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include "MeshDataEtc.h"
using namespace std;

class MeshData
{
public:
	MeshData();
	~MeshData();
public:
	string		meshName;
	int			nMeterialID;

	string		parentName;
	bool		bParent;

	// -- ��� --
	D3DXMATRIXA16		matTM;
	D3DXMATRIXA16		matPos;
	D3DXMATRIXA16		matRot;
	D3DXMATRIXA16		matScale;
	D3DXMATRIXA16		matModelTM;

	//  --�����--
	D3DXMATRIXA16		matInvTM;
	D3DXMATRIXA16		matInvPos;
	D3DXMATRIXA16		matInvRot;
	D3DXMATRIXA16		matInvScale;
	D3DXMATRIXA16		matInvModelTM;


	// -- ��ġ/ũ��/ȸ�� --
	D3DXVECTOR3			vPos;
	D3DXVECTOR3			vAxisRot;
	D3DXVECTOR3			vScale;
	D3DXVECTOR3			vAxisScale;
	float				fRotate;
	float				fScale;

	// -- mesh ����-------
	int					nVertexCount;
	int					nFaceCount;
	int					nTextureCount;
	DWORD*				pRenderMeterial;
	D3DXVECTOR3*		pVertexBuff;
	D3DXVECTOR2*		pTexUVBuff;
	SFaceData*			pFaceBuff;
	vector<SPosAnimation>	svPosAnim;
	vector<SRotAnimation>	svRotAnim;

public:
	void	MakeMatrix();
	void	Release();


};

