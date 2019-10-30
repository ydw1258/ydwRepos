#include "ASEParser.h"
#include <stdio.h>

ASEParser::ASEParser()
{
}

ASEParser::~ASEParser()
{
}

void ASEParser::Load(const char* name, vector<MeshData*>& svMeshData, SMeterial** pMeterial)
{
	m_cFile.Open(name, true);

	char		szLine[256] = { 0, };

	m_strFolder = DecodeForderName(name);

	while (m_cFile.ReadLine(szLine))
	{
		char	szWord[128] = { 0, };
		sscanf(szLine, "%s", szWord);

		if (strcmp(szWord, "*COMMENT") == 0) continue;	//*COMMENT {'	//주석은 넘어가자.

		if (strcmp(szWord, "*SCENE") == 0)
		{
			DecodeSCENE();
		}

		if (strcmp(szWord, "*MATERIAL_LIST") == 0)
		{
			*pMeterial = DecodeMeterialList();
		}

		if (strcmp(szWord, "*GEOMOBJECT") == 0)
		{
			svMeshData.push_back(DecodeGEOMOBJECT());
		}
	}

	m_cFile.Close();


	//	SetLocalSpace(svMeshData);
}

void ASEParser::SetLocalSpace(vector<MeshData*>& svMeshData)
{
	/*for( int i = 0 ; i < svMeshData.size() ; i++)
	{
	for( int j = 0 ; j < svMeshData[i]->nVertexCount ; j++)
	{

	D3DXVec3TransformCoord( &svMeshData[i]->pVertexBuff[j],
	&svMeshData[i]->pVertexBuff[j], &svMeshData[i]->matInvTM );
	}
	}*/

}


void ASEParser::DecodeSCENE()
{
	char		szLine[256] = { 0, };
	m_cFile.ReadLine(szLine);	// 파일이름은 버린다
	m_cFile.ReadLine(szLine);	// 애니메이셔 정보 SCENE_FIRSTFRAME


	char		szTemp[128] = { 0, };
	sscanf(szLine, "%s%d", szTemp, &m_nFirstFrame);

	m_cFile.ReadLine(szLine);	// 애니메이셔 정보 SCENE_LASTFRAME
	sscanf(szLine, "%s%d", szTemp, &m_nLastFrame);

	m_cFile.ReadLine(szLine);	// 애니메이셔 정보 SCENE_FRAMESPEED
	sscanf(szLine, "%s%d", szTemp, &m_nFrameSpeed);

	m_cFile.ReadLine(szLine);	// 애니메이셔 정보 SCENE_TICKSPERFRAME
	sscanf(szLine, "%s%d", szTemp, &m_nTickPerFrame);

	m_cFile.ReadLine(szLine);	//*SCENE_BACKGROUND_STATIC	
	m_cFile.ReadLine(szLine);	//*SCENE_AMBIENT_STATIC	
	m_cFile.ReadLine(szLine);	//}
}


SMeterial* ASEParser::DecodeMeterialList()
{
	char		szLine[256] = { 0, };
	char		szTemp[256] = { 0, };
	m_cFile.ReadLine(szLine);

	int		nMaterialCount = 0;
	sscanf(szLine, "%s%d", szTemp, &nMaterialCount);
	SMeterial* pMeterial = 0;
	if (nMaterialCount > 0)
	{
		pMeterial = new SMeterial[nMaterialCount];
		pMeterial->nSize = nMaterialCount;
	}


	while (true)
	{
		m_cFile.ReadLine(szLine);

		char	szWord[128] = { 0, };
		int		nTemp;
		sscanf(szLine, "%s%d", szWord, &nTemp); //*MATERIAL 0 { Meteral 번호

		if (strcmp(szWord, "*MATERIAL") == 0)
		{
			DecodeMeterial(&pMeterial[nTemp]);
			continue;
		}

		if (strcmp(szWord, "}") == 0)
			break;
	}

	return pMeterial;
}


void ASEParser::DecodeMeterial(SMeterial* pMeterial)
{

	memset(pMeterial, 0, sizeof(SMeterial));
	float	fShine = 0.f;
	float	fTransParency = 0.f;
	float	fShineStrength = 0.f;
	while (true)
	{
		char		szLine[256] = { 0, };
		char		szTemp[256] = { 0, };
		char		szWord[256] = { 0, };


		m_cFile.ReadLine(szLine);		//*MATERIAL_NAME

		sscanf(szLine, "%s", szWord);


		if (strcmp(szWord, "*MATERIAL_AMBIENT") == 0)
		{
			sscanf(szLine, "%s%f%f%f", szTemp, &pMeterial->dxMeterial.Ambient.r,
				&pMeterial->dxMeterial.Ambient.g,
				&pMeterial->dxMeterial.Ambient.b);
		}

		if (strcmp(szWord, "*MATERIAL_DIFFUSE") == 0)
		{
			sscanf(szLine, "%s%f%f%f", szTemp, &pMeterial->dxMeterial.Diffuse.r,
				&pMeterial->dxMeterial.Diffuse.g,
				&pMeterial->dxMeterial.Diffuse.b);
		}


		if (strcmp(szWord, "*MATERIAL_SPECULAR") == 0)
		{
			sscanf(szLine, "%s%f%f%f", szTemp, &pMeterial->dxMeterial.Specular.r,
				&pMeterial->dxMeterial.Specular.g,
				&pMeterial->dxMeterial.Specular.b);
		}


		if (strcmp(szWord, "*MATERIAL_SHINE") == 0)
		{
			sscanf(szLine, "%s%f", szTemp, &fShine);
		}


		if (strcmp(szWord, "*MATERIAL_SHINESTRENGTH") == 0)
		{
			sscanf(szLine, "%s%f", szTemp, &fShineStrength);
		}


		if (strcmp(szWord, "*MATERIAL_TRANSPARENCY") == 0)
		{
			sscanf(szLine, "%s%f", szTemp, &fTransParency);
		}

		if (strcmp(szWord, "*MAP_DIFFUSE") == 0)
		{
			DecodeMapDiffuse(pMeterial);
		}

		if (strcmp(szWord, "}") == 0)
			break;
	}

	pMeterial->dxMeterial.Diffuse.a = 1.f - fTransParency;
	pMeterial->dxMeterial.Ambient.a = 1.f - fTransParency;
	pMeterial->dxMeterial.Specular.a = 1.f - fTransParency;

	D3DXCOLOR	dxSpecColor(pMeterial->dxMeterial.Specular.r, pMeterial->dxMeterial.Specular.g, pMeterial->dxMeterial.Specular.b, 1.f);
	pMeterial->dxMeterial.Specular = dxSpecColor * fShineStrength;
	pMeterial->dxMeterial.Power = fShine * 100.f;

	if (fShineStrength > 0.0f)
		pMeterial->bSpecular = true;

}


void ASEParser::DecodeMapDiffuse(SMeterial* pMeterial)
{
	char szLine[256] = { 0, };
	char szWord[256] = { 0, };
	char szTemp[256] = { 0, };

	while (true)
	{
		m_cFile.ReadLine(szLine);
		sscanf(szLine, "%s", szWord);

		if (strcmp(szWord, "*BITMAP") == 0)
		{
			string strFileName = DecodeFileName(szLine);
			pMeterial->strTexture = m_strFolder + strFileName;

			//		// 폴더는 제거하자
		}

		if (strcmp(szWord, "}") == 0)
			break;
	}
}


string	ASEParser::DecodeFileName(const char* source)
{
	string strSource = DecodeName(source);

	char	szFileName[128] = { 0, };
	int		nCount = 0;

	const char* temp = strSource.c_str();

	for (int i = strSource.length() - 1; i >= 0; i--)
	{
		if (temp[i] == '\\' || temp[i] == '/')
		{
			break;
		}

		szFileName[nCount++] = temp[i];
	}

	reverse(&szFileName[0], &szFileName[strlen(szFileName)]);

	return szFileName;
}


string	ASEParser::DecodeName(const char* source)
{
	int		nLength = strlen(source);
	char	szName[128] = { 0, };
	bool	bStart = false;
	int		nCount = 0;

	for (int i = 0; i < nLength; i++)
	{

		if (bStart)
		{
			if (source[i] == '"')
				break;

			szName[nCount++] = source[i];
		}

		if (source[i] == '"')
			bStart = true;
	}

	return szName;
}

string  ASEParser::DecodeForderName(const char* source)
{
	string strSource = DecodeName(source);

	const char* temp = strSource.c_str();
	char	szFolder[256];
	strcpy_s(szFolder, 256, source);

	for (int i = strlen(szFolder) - 1; i >= 0; i--)
	{
		if (szFolder[i] == '\\' || szFolder[i] == '/')
		{
			break;
		}

		szFolder[i] = 0;
	}

	//reverse(&szFileName[0], &szFileName[ strlen(szFileName) ] );

	return szFolder;
}


MeshData* ASEParser::DecodeGEOMOBJECT()
{
	MeshData*	pMeshData = new MeshData;


	while (true)
	{
		char		szLine[256] = { 0, };
		char		szTemp[256] = { 0, };
		m_cFile.ReadLine(szLine);

		char	szWord[128] = { 0, };
		sscanf(szLine, "%s", szWord);

		if (strcmp(szWord, "}") == 0)
			break;

		// 젤첫줄
		if (strcmp(szWord, "*NODE_NAME") == 0)
		{
			pMeshData->meshName = DecodeName(szLine);	continue;
		}

		// 젤첫줄
		if (strcmp(szWord, "*NODE_PARENT") == 0)
		{
			pMeshData->bParent = true;
			pMeshData->parentName = DecodeName(szLine);	continue;
		}

		if (strcmp(szWord, "*NODE_TM") == 0)
		{
			DecodeNodeTM(pMeshData);		continue;
		}

		if (strcmp(szWord, "*MESH") == 0)
		{
			DecodeMesh(pMeshData);	continue;
		}

		if (strcmp(szWord, "*TM_ANIMATION") == 0)
		{
			DecodeMeshAnimation(pMeshData);
		}

		if (strcmp(szWord, "*MATERIAL_REF") == 0)			//'*MATERIAL_REF 0'
		{
			sscanf(szLine, "%s %d", szTemp, &pMeshData->nMeterialID);
			continue;
		}

	}


	return pMeshData;
}


void ASEParser::DecodeNodeTM(MeshData* pMeshData)
{
	char		szTemp[128] = { 0, };
	char		szLine[256] = { 0, };
	m_cFile.ReadLine(szLine);			//'*NODE_NAME ...'
	m_cFile.ReadLine(szLine);			//'*INHERIT_POS ...'
	m_cFile.ReadLine(szLine);			//'*INHERIT_ROT ...'
	m_cFile.ReadLine(szLine);			//'*INHERIT_SCL ...'

										// TM행렬

										// y 와 z가 바뀌어있다
	m_cFile.ReadLine(szLine);

	D3DXMatrixIdentity(&pMeshData->matTM);

	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->matTM._11, &pMeshData->matTM._13, &pMeshData->matTM._12);

	m_cFile.ReadLine(szLine);
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->matTM._31, &pMeshData->matTM._33, &pMeshData->matTM._32);

	m_cFile.ReadLine(szLine);
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->matTM._21, &pMeshData->matTM._23, &pMeshData->matTM._22);

	m_cFile.ReadLine(szLine);
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->matTM._41, &pMeshData->matTM._43, &pMeshData->matTM._42);


	m_cFile.ReadLine(szLine);	//*TM_POS
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->vPos.x, &pMeshData->vPos.z, &pMeshData->vPos.y);


	m_cFile.ReadLine(szLine);	//*TM_ROTAXIS
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->vAxisRot.x, &pMeshData->vAxisRot.z, &pMeshData->vAxisRot.y);

	// 회전각도 // 
	m_cFile.ReadLine(szLine);	//*TM_ROTANGLE 
	sscanf(szLine, "%s%f", szTemp, &pMeshData->fRotate);

	// 스케일값
	m_cFile.ReadLine(szLine);	//*TM_SCALE 
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->vScale.x, &pMeshData->vScale.z, &pMeshData->vScale.y);

	//스케일 축 
	m_cFile.ReadLine(szLine);	//*TM_SCALEAXIS 
	sscanf(szLine, "%s%f%f%f", szTemp, &pMeshData->vAxisScale.x, &pMeshData->vAxisScale.z, &pMeshData->vAxisScale.y);

	//스케일 축 각도값
	m_cFile.ReadLine(szLine);	//*TM_SCALEAXISANG 
	sscanf(szLine, "%s%f", szTemp, &pMeshData->fScale);

	pMeshData->MakeMatrix();
	m_cFile.ReadLine(szLine);	//}
}

void ASEParser::DecodeMesh(MeshData* pMeshData)
{
	char		szLine[256] = { 0, };
	char		szTemp[128] = { 0, };
	m_cFile.ReadLine(szLine);	// TIMEVALUE 무시

	m_cFile.ReadLine(szLine);	// MESH_NUMVERTEX
	sscanf(szLine, "%s%d", szTemp, &pMeshData->nVertexCount);

	m_cFile.ReadLine(szLine);	// MESH_NUMFACES
	sscanf(szLine, "%s%d", szTemp, &pMeshData->nFaceCount);

	if (pMeshData->nVertexCount > 0)
		pMeshData->pVertexBuff = new D3DXVECTOR3[pMeshData->nVertexCount];

	if (pMeshData->nFaceCount  > 0)
	{
		pMeshData->pFaceBuff = new SFaceData[pMeshData->nFaceCount];
		pMeshData->pRenderMeterial = new DWORD[pMeshData->nFaceCount];
		memset(pMeshData->pRenderMeterial, 0, sizeof(DWORD) *pMeshData->nFaceCount);
	}

	if (pMeshData->nVertexCount > 0)
	{
		m_cFile.ReadLine(szLine);	// MESH_VERTEX_LIST
		DecodeMeshVertexLIST(pMeshData);
	}

	if (pMeshData->nFaceCount  > 0)
	{
		m_cFile.ReadLine(szLine);	// MESH_FACE_LIST
		DecodeMeshFaceLIST(pMeshData);
	}

	while (true)
	{

		m_cFile.ReadLine(szLine);	// MESH_NUMTVERTEX
		char		szWord[128] = { 0, };
		sscanf(szLine, "%s", szWord);

		if (strcmp(szWord, "*MESH_NUMTVERTEX") == 0)
		{
			int nNumTVertex = 0;
			sscanf(szLine, "%s%d", szTemp, &pMeshData->nTextureCount);
			if (pMeshData->nTextureCount > 0)
			{
				pMeshData->pTexUVBuff = new D3DXVECTOR2[pMeshData->nTextureCount];
				DecodeMeshTVertexList(pMeshData);
			}
		}

		if (strcmp(szWord, "*MESH_NUMTVFACES") == 0)
		{
			int	nNumTFace = 0;
			sscanf(szLine, "%s%d", szTemp, &nNumTFace);
			if (nNumTFace > 0)
			{
				DecodeMeshTFaceList(pMeshData);
			}
		}

		if (strcmp(szWord, "*MESH_NORMALS") == 0)
		{
			DecodeMeshNormals(pMeshData);
		}

		if (strcmp(szWord, "}") == 0)
			break;
	}
}

void ASEParser::DecodeMeshTFaceList(MeshData* pMeshData)
{
	char	szLine[256] = { 0, };
	char	szTemp[256] = { 0, };
	m_cFile.ReadLine(szLine);		// *MESH_TFACELIST 

	for (int i = 0; i < pMeshData->nFaceCount; i++)
	{
		int		nIndex = 0;
		int	nF = 0, nS = 0, nT = 0;

		m_cFile.ReadLine(szLine);		// *MESH_TFACE 

		sscanf(szLine, "%s%d%d%d%d", szTemp, &nIndex, &nF, &nS, &nT);

		pMeshData->pFaceBuff[nIndex].nTIndex[0] = nF;
		pMeshData->pFaceBuff[nIndex].nTIndex[1] = nT;
		pMeshData->pFaceBuff[nIndex].nTIndex[2] = nS;
		int nX = 3;
	}

	m_cFile.ReadLine(szLine);		// }
}


void ASEParser::DecodeMeshTVertexList(MeshData* pMeshData)
{
	char	szLine[256] = { 0, };
	char	szTemp[256] = { 0, };
	m_cFile.ReadLine(szLine);		// *MESH_TVERTLIST 

	for (int i = 0; i < pMeshData->nTextureCount; i++)
	{
		m_cFile.ReadLine(szLine);		// *MESH_TVERT 

		int		nIndex = 0;
		float	fTemp = 0.f;
		D3DXVECTOR3	vUV(0, 0, 0);
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nIndex, &vUV.x, &vUV.z, &vUV.y/*, &vUV.z */);
		vUV.z = 1.f - vUV.z;
		pMeshData->pTexUVBuff[nIndex] = D3DXVECTOR2(vUV.x, vUV.z);
	}

	m_cFile.ReadLine(szLine);		// } 
}

void ASEParser::DecodeMeshNormals(MeshData* pMeshData)
{
	char		szLine[256] = { 0, };
	char		szTemp[256] = { 0, };
	int			nTemp = 0;
	int			nFaceNumber = 0;

	for (int i = 0; i < pMeshData->nFaceCount; i++)
	{
		D3DXVECTOR3	vNormal(0, 0, 0);
		m_cFile.ReadLine(szLine);	//*MESH_FACENORMAL
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nFaceNumber, &vNormal.x, &vNormal.z, &vNormal.y);
		pMeshData->pFaceBuff[nFaceNumber].vFaceNormal = vNormal;

		m_cFile.ReadLine(szLine);	//*MESH_VERTEXNORMAL
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nTemp, &vNormal.x, &vNormal.z, &vNormal.y);
		pMeshData->pFaceBuff[nFaceNumber].vNormal[0] = vNormal;


		m_cFile.ReadLine(szLine);	//*MESH_VERTEXNORMAL
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nTemp, &vNormal.x, &vNormal.z, &vNormal.y);
		pMeshData->pFaceBuff[nFaceNumber].vNormal[2] = vNormal;


		m_cFile.ReadLine(szLine);	//*MESH_VERTEXNORMAL
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nTemp, &vNormal.x, &vNormal.z, &vNormal.y);
		pMeshData->pFaceBuff[nFaceNumber].vNormal[1] = vNormal;
	}

	m_cFile.ReadLine(szLine);	//}

}


void ASEParser::DecodeMeshVertexLIST(MeshData* pMeshData)
{
	char		szLine[256] = { 0, };
	char		szTemp[128] = { 0, };

	for (int i = 0; i < pMeshData->nVertexCount; i++)
	{
		m_cFile.ReadLine(szLine);	// MESH_VERTEX
		int		nIndex = -1;
		D3DXVECTOR3	vPos(0, 0, 0);
		sscanf(szLine, "%s%d%f%f%f", szTemp, &nIndex, &vPos.x, &vPos.z, &vPos.y);

		D3DXVec3TransformCoord(&vPos, &vPos, &pMeshData->matInvTM);

		pMeshData->pVertexBuff[nIndex] = vPos;
	}

	m_cFile.ReadLine(szLine);	// }
}

void ASEParser::DecodeMeshFaceLIST(MeshData* pMeshData)
{
	char		szLine[256] = { 0, };
	char		szTemp[128] = { 0, };
	int			nTemp = 0;

	for (int i = 0; i < pMeshData->nFaceCount; i++)
	{
		/**MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1*/
		m_cFile.ReadLine(szLine);
		sscanf(szLine, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d",
			szTemp, // *MESH_FACE 
			szTemp, // 0: 
			szTemp, &pMeshData->pFaceBuff[i].nIndex[0],  // A: Value
			szTemp, &pMeshData->pFaceBuff[i].nIndex[2],  // B: Value
			szTemp, &pMeshData->pFaceBuff[i].nIndex[1],  // C: Value
			szTemp, &nTemp,  // AB : Value
			szTemp, &nTemp,  // BC : Value
			szTemp, &nTemp,  // CA : Value
			szTemp, szTemp,
			szTemp, szTemp, nTemp);
	}

	m_cFile.ReadLine(szLine);	// }
}



void ASEParser::DecodeMeshAnimation(MeshData* pMeshData)
{
	char	szLine[256] = { 0, };
	char	szWord[128] = { 0, };

	m_cFile.ReadLine(szLine);	//	*NODE_NAME
	m_cFile.ReadLine(szLine);	//	*CONTROL_POS_TRACK

	sscanf(szLine, "%s", szWord);

	if (strcmp(szWord, "*CONTROL_POS_TRACK") == 0)
	{
		while (true)
		{
			m_cFile.ReadLine(szLine);


			char	szTemp[128] = { 0, };
			sscanf(szLine, "%s", szWord);

			if (strcmp(szWord, "*CONTROL_POS_SAMPLE") == 0)
			{
				SPosAnimation	sPos;
				sscanf(szLine, "%s%d%f%f%f", szTemp, &sPos.nTime, &sPos.vPosition.x, &sPos.vPosition.z, &sPos.vPosition.y);
				pMeshData->svPosAnim.push_back(sPos);
			}

			if (strcmp(szWord, "}") == 0)
				break;
		}

		m_cFile.ReadLine(szLine);	//	*CONTROL_ROT_TRACK
	}

	sscanf(szLine, "%s", szWord);

	if (strcmp(szWord, "*CONTROL_ROT_TRACK") == 0)
	{

		while (true)
		{
			m_cFile.ReadLine(szLine);

			char	szTemp[128] = { 0, };
			sscanf(szLine, "%s", szWord);

			if (strcmp(szWord, "*CONTROL_ROT_SAMPLE") == 0)
			{
				SRotAnimation sRot;
				D3DXVECTOR4	vRot;
				sscanf(szLine, "%s%d%f%f%f%f", szTemp, &sRot.nTime, &vRot.x, &vRot.z, &vRot.y, &vRot.w);

				sRot.qRotate.x = sinf(vRot.w / 2.f) * vRot.x;
				sRot.qRotate.y = sinf(vRot.w / 2.f) * vRot.y;
				sRot.qRotate.z = sinf(vRot.w / 2.f) * vRot.z;
				sRot.qRotate.w = cosf(vRot.w / 2.f);

				if (pMeshData->svRotAnim.size() == 0)
				{
					pMeshData->svRotAnim.push_back(sRot);
				}
				else
				{
					SRotAnimation		sTail = pMeshData->svRotAnim.back();
					D3DXQuaternionMultiply(&sRot.qRotate, &sTail.qRotate, &sRot.qRotate);
					pMeshData->svRotAnim.push_back(sRot);
				}
			}

			if (strcmp(szWord, "}") == 0)
				break;
		}
	}

}
