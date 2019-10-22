#pragma once
#include<Windows.h>

class Physics
{
private:
	static Physics* mthis;
	DWORD frameCount = 0;            //������ ī��Ʈ��
	float timeElapsed = 0.0f;            //�帥 �ð�
	DWORD lastTime = timeGetTime();   //������ �ð�(temp����)

	LARGE_INTEGER NextCount;
	LARGE_INTEGER CurCount;
public:
	float deltaTime; //���� ��
	void deltaTimeInit();
	static Physics* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Physics();
		return mthis;
	}
	void Release();
	bool RECTbyRECTCollisionCheck(RECT rc, RECT rc2);
	bool RECTbyPointCollisionCheck(RECT rc, POINT pt);

	//Gravity
	//Timer
	void accumulate();
};