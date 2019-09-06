#include "Physics.h"

Physics* Physics::mthis = nullptr;

bool Physics::RECTbyRECTCollisionCheck(RECT rc, RECT rc2)
{
	RECT rcTemp;

	if (IntersectRect(&rcTemp, &rc, &rc2))
	{
		return true;
	}
	return false;
}

bool Physics::RECTbyPointCollisionCheck(RECT rc, POINT pt)
{
	if (PtInRect(&rc, pt))
	{
		return true;
	}
	return false;
}

void Physics::accumulate()
{
	// ��ŸŸ���� ���Ѵ�.
	
	DWORD curTime = timeGetTime();      //���� �ð�
	deltaTime = (curTime - lastTime) * 0.001f;
	
	/*
	QueryPerformanceCounter(&NextCount);
	deltaTime = ((float)(NextCount.QuadPart - CurCount.QuadPart)) / ((float)CpuFrequency.QuadPart);
	CurCount.QuadPart = NextCount.QuadPart;

	// FPS ��� 1�ʴ� ������ ��...
	AccCal_Time += deltaTime;
	AccumulateTime += deltaTime;
	*/
}