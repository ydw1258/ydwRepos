#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
using namespace std;

class Animation;
class Mesh
{
private:
	vector<D3DXVECTOR3>				m_svVertex;
	vector<D3DXVECTOR3>				m_svNormal;
	vector<D3DXVECTOR2>				m_svUVBuffer;
	vector<DWORD>					m_svColor;
	vector<WORD>					m_svIndex;

	LPDIRECT3DTEXTURE9				m_pTexture;
	D3DMATERIAL9					m_dxMaterial;
	DWORD							m_dwFVF;
	LPD3DXMESH						m_dxMesh;


	D3DXMATRIXA16					m_matCurTM;
	D3DXMATRIXA16					m_matTM;	
	D3DXMATRIXA16					m_matInvTM;	 

	bool							m_bBone;
	string							m_meshName;
	Mesh*							m_pParent;
	vector<Mesh*>					m_svChild;
	Animation*						m_pAnimation;

protected:
	int			GetFVFSize();
	void		MakeVertexBuffer(LPD3DXMESH pSysMesh);
	void		MakeIndexBuffer(LPD3DXMESH pSysMesh);
	void		CreateAttributeBuffer(LPDIRECT3DDEVICE9 pDevice , LPD3DXMESH pSysMesh);
	void		OptimizeMesh(LPDIRECT3DDEVICE9 pDevice, LPD3DXMESH pSysMesh, LPD3DXBUFFER pAdjacencyBuffer, LPD3DXMESH* ppOptimizedMesh);

public:
	void		SetName(const string name) { m_meshName = name; }
	void		SetFVF(DWORD dwFVF) { m_dwFVF = dwFVF; }
	void		AddVertexBuffer(D3DXVECTOR3 vBuf) { m_svVertex.push_back(vBuf); }
	void		AddNormalBuffer(D3DXVECTOR3 vBuf) { m_svNormal.push_back(vBuf); }
	void		AddIndexBuffer(WORD index) { m_svIndex.push_back(index); }
	void		AddIndexBuffer(WORD n1, WORD n2, WORD n3) { m_svIndex.push_back(n1); m_svIndex.push_back(n2); m_svIndex.push_back(n3); }
	void		AddColor(DWORD dwColor) { m_svColor.push_back(dwColor); }
	void		AddUVBuffer(D3DXVECTOR2 vUV) { m_svUVBuffer.push_back(vUV); }
	void		SetMaterial(D3DMATERIAL9 mtrl) { m_dxMaterial = mtrl; }
	
	void		Init(LPDIRECT3DDEVICE9 device);

	void		SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	void		SetTM(D3DXMATRIXA16& mat) { m_matTM = mat; }
	void		SetInvTM(D3DXMATRIXA16& mat) { m_matInvTM = mat; }
	void		SetParent(Mesh* pMesh) { m_pParent = pMesh; }
	void		AddChild(Mesh* pMesh) { m_svChild.push_back(pMesh); }
	void		SetBone(bool bBone) { m_bBone = bBone; }

	D3DXVECTOR3	GetVertex(int nIdx) { return m_svVertex[nIdx]; }
	D3DXVECTOR3	GetNormal(int nIdx) { return m_svNormal[nIdx]; }
	int			GetVertexCount() { return m_dxMesh != nullptr ? m_dxMesh->GetNumVertices() : m_svVertex.size(); }
	string		GetName() { return m_meshName; }
	Mesh*		GetParent() { return m_pParent; }
	const D3DXMATRIXA16& GetInvTM() { return m_matInvTM; }
	const D3DXMATRIXA16& GetTM() { return m_matTM; }

	void		SetAnimation(Animation* pAnim) { m_pAnimation = pAnim; }
protected:
	bool		IsVisible();
public:
	void		Update(D3DXMATRIXA16& matTM, float fFrame = 0);
	void		DrawMesh(LPDIRECT3DDEVICE9 device);
	void		Release();
	Mesh();
	virtual ~Mesh();
};

