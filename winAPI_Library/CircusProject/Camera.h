#pragma once
#include"Windows.h"
#include"ResourceManager.h"

class Camera
{
private:
	static Camera* mthis;
	int x;
	int y;
	int targetX;
	int targetY;
public:
	static Camera* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Camera();
		return mthis;
	}
	void Render(HDC hdc);
};
