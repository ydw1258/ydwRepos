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
		///< 에러 처리 
		return E_FAIL;
	}

	///< 키보드 입력 디바이스 생성
	if (m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyDev, NULL) != DI_OK)
	{
		return E_FAIL;
	}

	//디바이스의 데이터 포맷 설정.
	if (m_pKeyDev->SetDataFormat(&c_dfDIKeyboard) != DI_OK)
	{
		return E_FAIL;
	}

	// 키보드 디바이스 협력 레벨을 설정
	// DISCL_EXCLUSIVE    - 딴 프로그램에서는 Aquire() 하고 있는 동안에 키보드를 사용할 수가 없다.
	// DISCL_NONEXCLUSIVE - 딴 프로그램과 키보드 정보를 공유한다.
	// DISCL_FOREGROUND   - 윈도우에 Focus가 있을 때만 키보드 정보를 받을 수 있다.
	// DISCL_BACKGROUND   - 윈도우가 비활성되어도 키보드 정보를 받을 수 있다.

	// DISCL_EXCLUSIVE 와 DISCL_NONEXCLUSIVE 는 동시에 사용할 수 없고
	// DISCL_FOREGROUND 와 DISCL_BACKGROUND 도 마찬가지다. 
	if (FAILED(hr = m_pKeyDev->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY)))
	{
		return E_FAIL;
	}

	/// 디바이스 접근 제어얻기
	if (m_pKeyDev->Acquire() != DI_OK)
	{
		return E_FAIL;
	}

	///< 마우스 입력 디바이스 생성
	if (m_pInput->CreateDevice(GUID_SysMouse, &m_pInputMouse, NULL) != DI_OK)
	{
		return E_FAIL;
	}

	///< 디바이스의 데이터 포맷 설정
	if (m_pInputMouse->SetDataFormat(&c_dfDIMouse) != DI_OK)
	{
		return E_FAIL;
	}

	///< 마우스 디바이스의 렵력 레벨 설정
	if (m_pInputMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK)
	{
		return E_FAIL;
	}

	///< 디바이스 접근 제어권 얻기
	if (m_pInputMouse->Acquire() != DI_OK)
	{
		return E_FAIL;
	}

	return S_OK;
}

void InputManager::Update()
{
	///< 키보드 상태 처리 
	///< 현재 키보드버퍼의 상태를 이전의 상태로 복사한다.
	memcpy(m_PreKeyState, m_CurKeyState, sizeof(BYTE) * 256);
	///< 디바이스의 상태을 파악해서 UnBuffer한다

	if (m_pKeyDev && m_pKeyDev->GetDeviceState(256, (LPVOID)m_CurKeyState))
	{
		///< 초기화
		memset(m_CurKeyState, 0, sizeof(BYTE)* 256);
		///< 키보드 제어권 얻기
		m_pKeyDev->Acquire();
	}
	///< 마우스 상태 처리 
	///< 현재 마우스의 상태를 이전의 상태로 백업 
	memcpy(&m_PreMouseState, &m_CurMouseState, sizeof(DIMOUSESTATE));
	///< 마우스의 현재 상태를 얻는다.
	if (m_pInputMouse && m_pInputMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_CurMouseState))
	{
		///< 초기화 
		memset(&m_CurMouseState, 0, sizeof(DIMOUSESTATE));
		m_pInputMouse->Acquire();
	}
	///< 마우스 이동량 얻기.
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
	///< 키 제어권 반납 및 해제 
	if (m_pKeyDev)
	{
		m_pKeyDev->Unacquire();
		m_pKeyDev->Release();
		m_pKeyDev = NULL;
	}
	///< 마우스 제어권 반납 및 해제
	if (m_pInputMouse)
	{
		m_pInputMouse->Unacquire();
		m_pInputMouse->Release();
		m_pInputMouse = NULL;
	}
	///< DXInput 해제
	if (m_pInput)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

	SAFE_DELETE(m_pThis);
}