#pragma once
class Camera
{
private:
	static Camera* mthis;
public:
	int cameraX = 0;
	int cameraY = 0;

	static Camera* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new Camera();
		return mthis;
	}
	Camera();
	~Camera();
};
//SingletonTest::GetInstance()->Func(); //함수 호출시