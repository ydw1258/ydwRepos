#include "Physics.h"

Physics* Physics::mthis = nullptr;

void Physics::deltaTimeInit()
{
	// 델타타임을 구한다.
	DWORD curTime = timeGetTime();      //현재 시간
	//deltaTime = 0.015f;
	deltaTime = (curTime - lastTime) * 0.001f;
	lastTime = curTime;
}