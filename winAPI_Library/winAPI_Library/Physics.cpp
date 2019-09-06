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
	// 델타타임을 구한다.
	
	DWORD curTime = timeGetTime();      //현재 시간
	deltaTime = (curTime - lastTime) * 0.001f;
	
	/*
	QueryPerformanceCounter(&NextCount);
	deltaTime = ((float)(NextCount.QuadPart - CurCount.QuadPart)) / ((float)CpuFrequency.QuadPart);
	CurCount.QuadPart = NextCount.QuadPart;

	// FPS 계산 1초당 프레임 수...
	AccCal_Time += deltaTime;
	AccumulateTime += deltaTime;
	*/
}