#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <d3d.h>
#include <vector>
#include "FileIO.h"
#include "MeshData.h"
using namespace std;

class ASEParser
{
public:
	ASEParser();
	~ASEParser();

protected:
	FileIO			m_cFile;
	string			m_strFolder;

	// SCENEÁ¤º¸
public:
	int				m_nFrameSpeed;
	int				m_nTickPerFrame;
	int				m_nFirstFrame;
	int				m_nLastFrame;

public:

	void		Load(const char* name, vector<MeshData*>& svMeshData, SMeterial** pMeterial);

protected:
	void		DecodeSCENE();

	//---- Meterial Parsing--------
	SMeterial*	DecodeMeterialList();
	void		DecodeMeterial(SMeterial* pMeterial);
	void		DecodeMapDiffuse(SMeterial* pMeterial);

	//---- string Parsing--------
	string  	DecodeForderName(const char* source);
	string		DecodeFileName(const char* source);
	string		DecodeName(const char* source);

	//---- mesh Parsing----------

	MeshData*	DecodeGEOMOBJECT();
	void		DecodeMesh(MeshData* pMeshData);
	void		DecodeNodeTM(MeshData* pMeshData);

	void		DecodeMeshVertexLIST(MeshData* pMeshData);
	void		DecodeMeshFaceLIST(MeshData* pMeshData);
	void		DecodeMeshNormals(MeshData* pMeshData);

	void		DecodeMeshTVertexList(MeshData* pMeshData);
	void		DecodeMeshTFaceList(MeshData* pMeshData);

	void		DecodeMeshAnimation(MeshData* pMeshData);

protected:
	void		SetLocalSpace(vector<MeshData*>& svMeshData);
};

