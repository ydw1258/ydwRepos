#include "Camera.h"
#include "LabelRenderer.h"
#include "InputManager.h"

Camera::Camera()
{
	m_fCamMoveSpeed = 30.f;
}


Camera::~Camera()
{
}


void Camera::Init(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 vEye, D3DXVECTOR3 vLook, float fFOV, float fAspect, float fNear, float fFar)
{
	m_vEye = vEye;
	m_vLook = vLook;
	m_fFOV = D3DXToRadian(fFOV);
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	SetView(pDevice);
	SetProj(pDevice);
}


void Camera::Render()
{
	LabelRenderer::GetInstance()->DrawLabel(10, 10, D3DXCOLOR(1, 0, 0, 1), "Eye: %0.1f, %0.1f, %0.1f", m_vEye.x, m_vEye.y, m_vEye.z);
	LabelRenderer::GetInstance()->DrawLabel(10, 25, D3DXCOLOR(1, 0, 0, 1), "Look: %0.1f, %0.1f, %0.1f", m_vLook.x, m_vLook.y, m_vLook.z);
	LabelRenderer::GetInstance()->DrawLabel(10, 40, D3DXCOLOR(1, 0, 0, 1), "FOV : %0.1f", D3DXToDegree(m_fFOV));
	LabelRenderer::GetInstance()->DrawLabel(10, 55, D3DXCOLOR(1, 0, 0, 1), "CamSpeed : %0.1f", m_fCamMoveSpeed);
}

void Camera::Update(LPDIRECT3DDEVICE9 pDevice , float fEllipseTime)
{
	if (InputManager::GetInstance()->OnceKeyDown(DIK_ADD))
		m_fCamMoveSpeed += 2.f;

	if (InputManager::GetInstance()->OnceKeyDown(DIK_SUBTRACT))
		m_fCamMoveSpeed -= 2.f;


	if (InputManager::GetInstance()->KeyDown(DIK_A))
	{
		Side(pDevice , -m_fCamMoveSpeed * fEllipseTime);
	}

	if (InputManager::GetInstance()->KeyDown(DIK_D))
	{
		Side(pDevice, m_fCamMoveSpeed * fEllipseTime);
	}

	if (InputManager::GetInstance()->KeyDown(DIK_W))
	{
		Foward(pDevice, m_fCamMoveSpeed * fEllipseTime);
	}

	if (InputManager::GetInstance()->KeyDown(DIK_S))
	{
		Foward(pDevice, -m_fCamMoveSpeed * fEllipseTime);
	}

	if (InputManager::GetInstance()->MouseClick(MOUSE_MBUTTONDOWN))
	{
		const D3DXVECTOR3* vDelta = InputManager::GetInstance()->GetDelta();
		UpDown(pDevice, vDelta->y * 2 * fEllipseTime);
		Side(pDevice, vDelta->x * 2 * fEllipseTime);
	}

	if (InputManager::GetInstance()->MouseClick(MOUSE_LBUTTONDOWN))
	{
		const D3DXVECTOR3* vDelta = InputManager::GetInstance()->GetDelta();
		Fan(pDevice, (vDelta->x / 10.f) * fEllipseTime);
		Tilt(pDevice, (vDelta->y / 10.f) * fEllipseTime);
	}
}

void Camera::Foward(LPDIRECT3DDEVICE9 pDevice ,float fOffset)
{
	D3DXVECTOR3	vLookDir = m_vLook - m_vEye;
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	///< Eye를 갱신한다.
	m_vEye += vLookDir * fOffset;
	m_vLook = m_vEye + vLookDir * 1.f;

	SetView(pDevice);
}

void Camera::Side(LPDIRECT3DDEVICE9 pDevice, float fOffset)
{
	D3DXVECTOR3	vLookDir = m_vLook - m_vEye;
	D3DXVECTOR3 vUp(0, 1, 0);
	D3DXVECTOR3	vCross;
	D3DXVec3Cross(&vCross, &vUp, &vLookDir);
	D3DXVec3Normalize(&vCross, &vCross);

	m_vEye += vCross * fOffset;
	m_vLook += vCross * fOffset;

	SetView(pDevice);
}

void Camera::Fan(LPDIRECT3DDEVICE9 pDevice, float fOffset)
{
	D3DXMATRIXA16	matRot;
	D3DXVECTOR3		vLookDir = m_vLook - m_vEye;

	D3DXMatrixRotationY(&matRot, fOffset);
	D3DXVec3TransformCoord(&vLookDir, &vLookDir, &matRot);
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	m_vLook = m_vEye + vLookDir;

	SetView(pDevice);
}

void Camera::Tilt(LPDIRECT3DDEVICE9 pDevice, float fOffset)
{
	D3DXMATRIXA16	matRot;
	D3DXVECTOR3		vLookDir = m_vLook - m_vEye;

	D3DXVECTOR3 vUp(0, 1, 0);
	D3DXVECTOR3	vCross;
	D3DXVec3Cross(&vCross, &vUp, &vLookDir);

	D3DXMatrixRotationAxis(&matRot, &vCross, fOffset);
	D3DXVec3TransformCoord(&vLookDir, &vLookDir, &matRot);
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	m_vLook = m_vEye + vLookDir;

	SetView(pDevice);
}

void Camera::UpDown(LPDIRECT3DDEVICE9 pDevice, float fOffset)
{
	D3DXVECTOR3 vUp(0, 1, 0);

	m_vEye += vUp * -fOffset;
	m_vLook += vUp * -fOffset;

	SetView(pDevice);
}

void Camera::SetView(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXVECTOR3	vUp(0, 1, 0);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLook, &vUp);
	pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

void Camera::SetProj(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFOV, m_fAspect, m_fNear, m_fFar);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}