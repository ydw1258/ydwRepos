#include "Timer.h"



Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Init()
{
	///< ����Ÿ�̸� �������� üũ
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_nPeriodFrequency))
	{
		m_bIsHardWare = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nLastTime);
		m_fTimeScale = 1.0f / m_nPeriodFrequency;
	}
	else
	{
		m_bIsHardWare = false;
		m_nLastTime = GetTickCount();
		m_fTimeScale = 1.0f;
	}
	m_nFrameRate = 0;
	m_nFPSFrameCount = 0;
	m_fWorldTime = 0;
	m_fFPSTimeElapsed = 0.0f;
}


void Timer::Update()
{
	//����ð��� ���´�.
	if (m_bIsHardWare)
	{
		///< �����е� ����ũ����(������ ��� )
		///< QueryPerformanceCounter �� ��Ƽ�ھ�� ����ȭ ������ �ִ�.
		QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurTime);
	}
	else
	{
		///< �������� ������� Time�Լ��� ����Ѵ�.
		m_nCurTime = GetTickCount();
	}
	///< ������ �ð��� ����ð��� ����� ����
	m_fTimeElapsed = (m_nCurTime - m_nLastTime) * m_fTimeScale;

	///< ������ �ð��� ���.
	m_nLastTime = m_nCurTime;

	///< �ʴ� ������ ī��Ʈ ����
	m_nFPSFrameCount++;

	///< FPS�ð� ����� ���� 
	m_fFPSTimeElapsed += m_fTimeElapsed;

	// ���� �������� �帥�ð�
	m_fWorldTime += m_fTimeElapsed;

	///< ������ �ʱ�ȭ(1��üũ)
	if (m_fFPSTimeElapsed > 1.0f)
	{
		m_nFrameRate = m_nFPSFrameCount;
		m_nFPSFrameCount = 0;
		m_fFPSTimeElapsed = 0.0f;
	}
}