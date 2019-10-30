#include "MeshData.h"
#include "SAFE_DELETE.h"

MeshData::MeshData()
{
	pRenderMeterial = 0;
	pVertexBuff = 0;
	pTexUVBuff = 0;
	pFaceBuff = 0;
	nMeterialID = -1;
	parentName = "";
	bParent = false;
}


MeshData::~MeshData()
{
}

void MeshData::MakeMatrix()	// 지금은 쓸일없다	그냥 미리 만들어놓음
{
	D3DXMatrixRotationAxis(&matRot, &vAxisRot, fRotate);

	D3DXMATRIXA16	matScaleTemp;
	D3DXMatrixScaling(&matScaleTemp, vScale.x, vScale.y, vScale.z);

	D3DXMATRIX matScaleRotAxis;		//스케일 회전축-회전행렬..
	D3DXMatrixRotationAxis(&matScaleRotAxis, &vAxisScale, fScale);

	D3DXMATRIX matScaleRotAxisInv;
	D3DXMatrixInverse(&matScaleRotAxisInv, 0, &matScaleRotAxis);

	matScale = matScaleRotAxis * matScaleTemp * matScaleRotAxisInv;

	D3DXMatrixTranslation(&matPos, vPos.x, vPos.y, vPos.z);

	matModelTM = matScale * matRot * matPos;


	D3DXMatrixInverse(&matInvScale, 0, &matScale);
	D3DXMatrixInverse(&matInvRot, 0, &matRot);
	D3DXMatrixInverse(&matInvPos, 0, &matPos);
	D3DXMatrixInverse(&matInvModelTM, 0, &matModelTM);


	D3DXMatrixInverse(&matInvTM, 0, &matTM);
}

void MeshData::Release()
{
	SAFE_DELETE_ARRAY(pVertexBuff);
	SAFE_DELETE_ARRAY(pFaceBuff);
}