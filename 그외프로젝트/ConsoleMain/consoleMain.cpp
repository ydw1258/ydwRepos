#include<iostream>
#include<assert.h>
#include<Windows.h>
using namespace std;

int main()
{
	//assert(n != 10 && "n�� 10�̸� ���� �߻�"); //����� ���� ���
	//srand(GetTickCount());
	LARGE_INTEGER CpuFrequency;
	LARGE_INTEGER CurCount;

	int n = 10;
	int* p;

	while (true)
	{
		cout << QueryPerformanceFrequency(&CpuFrequency); //�ʴ� Ŭ���� 
		system("cls");
		cout << QueryPerformanceCounter(&CurCount);
	}

	//ks���� ��ǲ
	/*
	bool is_press(const int& _Value)
	{
		SHORT X = GetAsyncKeyState(_Value); //�����ܴ�
		if (0 == X)
		{
			return false;
		}

		return true;
	}*/
	//QueryPerformanceCounter(&CurCount);
}
void start()
{
	LARGE_INTEGER CpuFrequency;
	LARGE_INTEGER CurCount;
	QueryPerformanceFrequency(&CpuFrequency);
	// ����Ÿ�� 
	QueryPerformanceCounter(&CurCount);

	AccumulateTime = .0f;
	AccCal_Time = .0f;
	FPSCount = 0;
}

void accumulate()
{
	// ��ŸŸ���� ���Ѵ�.
	QueryPerformanceCounter(&NextCount);

	DeltaTime = ((float)(NextCount.QuadPart - CurCount.QuadPart)) / ((float)CpuFrequency.QuadPart);
	CurCount.QuadPart = NextCount.QuadPart;

	// FPS ��� 1�ʴ� ������ ��...
	AccCal_Time += DeltaTime;
	AccumulateTime += DeltaTime;

	++FPSCount;
	if (1.0f <= AccCal_Time)
	{
		Fps = ((float)FPSCount / AccCal_Time);
		AccCal_Time = 0.0f;
		FPSCount = 0;
	}
}