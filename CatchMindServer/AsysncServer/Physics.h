#pragma once
#include<Windows.h>

class Physics
{
private:
	static Physics* mthis;
	DWORD frameCount = 0;            //������ ī��Ʈ��
	DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

public:
	float deltaTime; //���� ��
	void deltaTimeInit();
	static Physics* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Physics();
		return mthis;
	}

};