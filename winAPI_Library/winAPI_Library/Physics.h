#pragma once
#include<Windows.h>

class Physics
{
private:
	static Physics* mthis;
	DWORD frameCount = 0;            //������ ī��Ʈ��
	float timeElapsed = 0.0f;            //�帥 �ð�
	DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

public:
	float deltaTime;
	static Physics* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Physics();
		return mthis;
	}

	bool RECTbyRECTCollisionCheck(RECT rc, RECT rc2);
	bool RECTbyPointCollisionCheck(RECT rc, POINT pt);
	
	//Gravity
	//Timer
	void accumulate();
};