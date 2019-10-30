#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

enum MOUSE_STATE
{
	MOUSE_LBUTTONDOWN = 0,		///< L버튼 
	MOUSE_RBUTTONDOWN,
	MOUSE_MBUTTONDOWN
};

class InputManager
{
private:
	static InputManager*	m_pThis;

	LPDIRECTINPUT8			m_pInput;
	LPDIRECTINPUTDEVICE8	m_pKeyDev;

	BYTE					m_CurKeyState[256], m_PreKeyState[256];

	LPDIRECTINPUTDEVICE8	m_pInputMouse;
	DIMOUSESTATE			m_CurMouseState, m_PreMouseState;

	D3DXVECTOR2				m_MousePos;
	D3DXVECTOR3				m_MouseDelta;
	D3DXVECTOR3				m_KeyDelta;
	float					m_fMouseSmooth;
public:
	static InputManager*	GetInstance()
	{
		if (m_pThis == nullptr)
		{
			m_pThis = new InputManager();
		}

		return m_pThis;
	}

	HRESULT Init(HINSTANCE hInst, HWND hWnd);
	void Update();
	void Release();

	inline bool	KeyDown(DWORD key)
	{
		return (m_CurKeyState[key] & 0x80) ? true : false;
	}

	///< 키보드 업
	inline bool	KeyUp(DWORD key)
	{
		return (m_CurKeyState[key] & 0x80) ? false : true;
	}

	///< OnceKeyDonw
	inline bool	OnceKeyDown(DWORD key)
	{
		return ((m_CurKeyState[key] & 0x80) && !(m_PreKeyState[key] & 0x80)) ? true : false;
	}	//키보드 한번 클릭

		//마우스 클릭
	inline bool	MouseClick(int mou)
	{
		return (m_CurMouseState.rgbButtons[mou] & 0x80) ? true : false;
	}

	///< 마우스 업
	inline bool	MouseUp(int mou)
	{
		return (m_CurMouseState.rgbButtons[mou] & 0x80) ? false : true;
	}

	///< 마우스 한번 클릭
	inline bool	OnceMouseClick(int mou)
	{
		return ((m_CurMouseState.rgbButtons[mou] & 0x80)
			&& !(m_PreMouseState.rgbButtons[mou] & 0x80)) ? true : false;
	}

	inline void	SetMouseSmooth(float sm)
	{
		m_fMouseSmooth = sm;
	}
	inline void	SetMousePos(float x, float y)
	{
		m_MousePos.x = x; m_MousePos.y = y;
	}
	//마우스 이동량
	inline const D3DXVECTOR3* GetDelta()	const
	{
		return &m_MouseDelta;
	}
	inline const D3DXVECTOR2* GetPos()		const
	{
		return &m_MousePos;
	}

	InputManager();
	~InputManager();
};

