#pragma once
#include"Windows.h"
#include"ResourceManager.h"

class Camera
{
private:
	static Camera* mthis;
	int cameraX;
	int cameraY;
public:
	static Camera* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Camera();
		return mthis;
	}
	void Render(HDC hdc);
};
