#include "Physics.h"

Physics* Physics::mthis = nullptr;

void Physics::deltaTimeInit()
{
	// ��ŸŸ���� ���Ѵ�.
	DWORD curTime = timeGetTime();      //���� �ð�
	//deltaTime = 0.015f;
	deltaTime = (curTime - lastTime) * 0.001f;
	lastTime = curTime;
}