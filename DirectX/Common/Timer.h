#pragma once
#include <Windows.h>

class Timer
{
private:
	bool			m_bIsHardWare;
	float			m_fTimeScale;
	float			m_fTimeElapsed;
	float			m_fWorldTime;

	__int64			m_nCurTime;			///< �����ð�.
	__int64			m_nLastTime;		///< �����ð�.
	__int64			m_nPeriodFrequency;	///< �ð��� �ֱ�.

	unsigned long	m_nFrameRate;			///< FPS
	unsigned long	m_nFPSFrameCount;		///< FPS ī��Ʈ
	float			m_fFPSTimeElapsed;

public:
	Timer();
	~Timer();

	void			Init();
	///< ���� �ð��� ����Ѵ�.
	void			Update();

	///< ���� FPS�� ���´�.
	unsigned long	GetFrameRate() const
	{
		return m_nFrameRate;
	}

	///< ������ �ð��� ����ð��� ������� �����Ѵ�.
	inline float			GetElapsedTime() const
	{
		return m_fTimeElapsed;
	}

	inline float			GetTime() const
	{
		return m_fWorldTime;
	}
};

