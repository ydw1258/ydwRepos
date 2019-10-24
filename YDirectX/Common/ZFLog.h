#ifndef _ZFLOG_H_
#define _ZFLOG_H_

#include <windows.h>
#include <stdio.h>

#define ZF_LOG_TARGET_CONSOLE			0x00000001
#define ZF_LOG_TARGET_FILE				0x00000002
#define ZF_LOG_TARGET_WINDOW			0x00000004
#define ZF_LOG_TARGET_ALL				(ZF_LOG_TARGET_CONSOLE|ZF_LOG_TARGET_FILE|ZF_LOG_TARGET_WINDOW)
#define ZF_LOG_WINDOW_TITLE				"ZEngine F-Killer"
#define ZF_LOG_WINDOW_CX				300
#define ZF_LOG_WINDOW_CY				600

class ZFLog
{
	static ZFLog*	m_pThis;
	unsigned int	m_nTarget;
	char			m_szFilename[MAX_PATH];
	HWND			m_hwnd;
	HWND			m_hwndList;
public:
			
			~ZFLog();
	void	CreateLogWindow();
	int		Log(const char* fmt, ... );


public:
	static ZFLog*	GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new ZFLog(ZF_LOG_TARGET_WINDOW);
		}

		return m_pThis;
	}

	static void Release()
	{
		if (m_pThis)
		{
			delete m_pThis;
		}

		m_pThis = NULL;
	}
private:
	ZFLog(UINT32 nTarget, LPSTR szFilename = NULL);
	static	LRESULT CALLBACK WndProcZFLog( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};
#endif // _ZFLOG_H_