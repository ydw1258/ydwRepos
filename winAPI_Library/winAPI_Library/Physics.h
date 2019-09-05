#pragma once
#include<Windows.h>

class Physics
{
private:
	static Physics* mthis;
	DWORD frameCount = 0;            //프레임 카운트수
	float timeElapsed = 0.0f;            //흐른 시간
	DWORD lastTime = timeGetTime();   //마지막 시간(temp변수)

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