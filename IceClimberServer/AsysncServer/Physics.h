#pragma once
#include<Windows.h>

class Physics
{
private:
	static Physics* mthis;
	DWORD frameCount = 0;            //프레임 카운트수
	DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

public:
	float deltaTime; //단위 초
	void deltaTimeInit();
	static Physics* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Physics();
		return mthis;
	}

};