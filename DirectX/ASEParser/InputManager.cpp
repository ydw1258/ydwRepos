#include "InputManager.h"
#include "SAFE_DELETE.h"
#include <assert.h>

InputManager* InputManager::m_pThis = nullptr;

InputManager::InputManager()
{
	m_pInput = nullptr;
	m_pKeyDev = nullptr;
	m_pInputMouse = nullptr;

	memset(m_CurKeyState, 0, sizeof(BYTE) * 256);
	memset(m_PreKeyState, 0, sizeof(BYTE) * 256);
	memset(&m_CurMouseState, 0, sizeof(DIMOUSESTATE));
	memset(&m_PreMouseState, 0, sizeof(DIMOUSESTATE));

	m_MousePos.x = 0;
	m_MousePos.y = 0;

	m_fMouseSmooth = 1;
}


InputManager::~InputManager()
{
}

HRESULT InputManager::Init(HINSTANCE hInst, HWND hWnd)
{
	HRESULT	hr;

	if (DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pInput, NULL) != DI_OK)
	{
		///< ���� ó�� 
		return E_FAIL;
	}

	///< Ű���� �Է� ����̽� ����
	if (m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDev, NULL) != DI_OK)
	{
		return E_FAIL;
	}

	//����̽��� ������ ���� ����.
	if (m_pKeyDev->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
	{
		return E_FAIL;
	}

	// Ű���� ����̽� ���� ������ ����
	// DISCL_EXCLUSIVE    - �� ���α׷������� Aquire() �ϰ� �ִ� ���ȿ� Ű���带 ����� ���� ����.
	// DISCL_NONEXCLUSIVE - �� ���α׷��� Ű���� ������ �����Ѵ�.
	// DISCL_FOREGROUND   - �����쿡 Focus�� ���� ���� Ű���� ������ ���� �� �ִ�.
	// DISCL_BACKGROUND   - �����찡 ��Ȱ���Ǿ Ű���� ������ ���� �� �ִ�.

	// DISCL_EXCLUSIVE �� DISCL_NONEXCLUSIVE �� ���ÿ� ����� �� ����
	// DISCL_FOREGROUND �� DISCL_BACKGROUND �� ����������. 
	if (FAILED(hr = m_pKeyDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY)))
	{
		return E_FAIL;
	}

	/// ����̽� ���� ������
	if (m_pKeyDev->Acquire() != DI_OK)
	{
		return E_FAIL;
	}

	///< ���콺 �Է� ����̽� ����
	if (m_pInput->CreateDevice(GUID_SysMouse, &m_pInputMouse, NULL) != DI_OK)
	{
		return E_FAIL;
	}

	///< ����̽��� ������ ���� ����
	if (m_pInputMouse->SetDataFormat(&c_dfDIMouse) != DI_OK)
	{
		return E_FAIL;
	}

	///< ���콺 ����̽��� �Ʒ� ���� ����
	if (m_pInputMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK)
	{
		return E_FAIL;
	}

	///< ����̽� ���� ����� ���
	if (m_pInputMouse->Acquire() != DI_OK)
	{
		return E_FAIL;
	}

	return S_OK;
}

void InputManager::Update()
{
	///< Ű���� ���� ó�� 
	///< ���� Ű��������� ���¸� ������ ���·� �����Ѵ�.
	memcpy(m_PreKeyState, m_CurKeyState, sizeof(BYTE) * 256);
	///< ����̽��� ������ �ľ��ؼ� UnBuffer�Ѵ�

	if (m_pKeyDev && m_pKeyDev->GetDeviceState(256, (LPVOID)m_CurKeyState))
	{
		///< �ʱ�ȭ
		memset(m_CurKeyState, 0, sizeof(BYTE)* 256);
		///< Ű���� ����� ���
		m_pKeyDev->Acquire();
	}
	///< ���콺 ���� ó�� 
	///< ���� ���콺�� ���¸� ������ ���·� ��� 
	memcpy(&m_PreMouseState, &m_CurMouseState, sizeof(DIMOUSESTATE));
	///< ���콺�� ���� ���¸� ��´�.
	if (m_pInputMouse && m_pInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_CurMouseState))
	{
		///< �ʱ�ȭ 
		memset(&m_CurMouseState, 0, sizeof(DIMOUSESTATE));
		m_pInputMouse->Acquire();
	}
	///< ���콺 �̵��� ���.
	D3DXVECTOR3 CurMouseDelta =
		D3DXVECTOR3((float)m_CurMouseState.lX,
		(float)m_CurMouseState.lY,
			(float)m_CurMouseState.lZ / 120.f);

	float fPercentOfNew = 1.0f / m_fMouseSmooth;

	if (MouseClick(MOUSE_LBUTTONDOWN) ||
		MouseClick(MOUSE_RBUTTONDOWN) ||
		MouseClick(MOUSE_MBUTTONDOWN))
	{
		m_MouseDelta = m_MouseDelta * (1.0f - fPercentOfNew) + CurMouseDelta * fPercentOfNew;
	}
	else
	{
		m_MouseDelta = CurMouseDelta;
	}

	if (D3DXVec3Length(&m_MouseDelta) <= 0.001f)
	{
		m_MouseDelta *= 0.0f;
	}
}

void InputManager::Release()
{
	///< Ű ����� �ݳ� �� ���� 
	if (m_pKeyDev)
	{
		m_pKeyDev->Unacquire();
		m_pKeyDev->Release();
		m_pKeyDev = NULL;
	}
	///< ���콺 ����� �ݳ� �� ����
	if (m_pInputMouse)
	{
		m_pInputMouse->Unacquire();
		m_pInputMouse->Release();
		m_pInputMouse = NULL;
	}
	///< DXInput ����
	if (m_pInput)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

	SAFE_DELETE(m_pThis);
}