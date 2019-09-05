#pragma once

class SingletonTest
{
private:
	static SingletonTest* mthis;
public:

	static SingletonTest* GetInstance()
	{
		if (mthis == nullptr)
			mthis = new SingletonTest();
		return mthis;
	}
	void Func();
};

//CPP에 선언 //기승이가 사기침
SingletonTest* SingletonTest::mthis = nullptr;

//SingletonTest::GetInstance()->Func(); //함수 호출시