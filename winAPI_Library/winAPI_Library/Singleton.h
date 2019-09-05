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

//CPP�� ���� //����̰� ���ħ
SingletonTest* SingletonTest::mthis = nullptr;

//SingletonTest::GetInstance()->Func(); //�Լ� ȣ���