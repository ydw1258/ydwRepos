#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include "Mesh.h"
#include "MeshData.h"
using namespace std;

class ASEMesh
{
private:
	vector<Mesh*>		m_svBoneMesh;
	vector<Mesh*>		m_svMesh;
	int					m_nVertexCount;
	int					m_nFaceCount;

	Animation*			m_pCurentAnim;

	DWORD			GetFVF(MeshData* pMesh);
	void			MakeHierarchy(vector<MeshData*>& svMeshData);
	MeshData*		GetMeshData(vector<MeshData*>& svMeshData, string strNode);
	Mesh*			FindNode(vector<Mesh*>& svNode, string strNode);
	Animation*		LoadAnim(vector<MeshData*>& svMeshData);

public:
	ASEMesh();
	~ASEMesh();

	void Update(D3DXMATRIXA16& matTM, float fEllipseTime);
	void Init(LPDIRECT3DDEVICE9 pDevice, const char* fileName);
	void Render(LPDIRECT3DDEVICE9 pDevice);
};

