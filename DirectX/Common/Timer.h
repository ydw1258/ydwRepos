#pragma once
#include <Windows.h>

class Timer
{
private:
	bool			m_bIsHardWare;
	float			m_fTimeScale;
	float			m_fTimeElapsed;
	float			m_fWorldTime;

	__int64			m_nCurTime;			///< 현지시간.
	__int64			m_nLastTime;		///< 이전시간.
	__int64			m_nPeriodFrequency;	///< 시간의 주기.

	unsigned long	m_nFrameRate;			///< FPS
	unsigned long	m_nFPSFrameCount;		///< FPS 카운트
	float			m_fFPSTimeElapsed;

public:
	Timer();
	~Timer();

	void			Init();
	///< 현재 시간을 계산한다.
	void			Update();

	///< 현재 FPS를 얻어온다.
	unsigned long	GetFrameRate() const
	{
		return m_nFrameRate;
	}

	///< 마지막 시간과 현재시간의 경과량을 누적한다.
	inline float			GetElapsedTime() const
	{
		return m_fTimeElapsed;
	}

	inline float			GetTime() const
	{
		return m_fWorldTime;
	}
};

