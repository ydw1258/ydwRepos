#include<iostream>
#include<assert.h>
#include<Windows.h>
using namespace std;

int main()
{
	//assert(n != 10 && "n이 10이면 에러 발생"); //통과할 조건 명시
	//srand(GetTickCount());
	LARGE_INTEGER CpuFrequency;
	LARGE_INTEGER CurCount;

	int n = 10;
	int* p;

	while (true)
	{
		cout << QueryPerformanceFrequency(&CpuFrequency); //초당 클럭수 
		system("cls");
		cout << QueryPerformanceCounter(&CurCount);
	}

	//ks엔진 인풋
	/*
	bool is_press(const int& _Value)
	{
		SHORT X = GetAsyncKeyState(_Value); //빠르단다
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
	// 누적타임 
	QueryPerformanceCounter(&CurCount);

	AccumulateTime = .0f;
	AccCal_Time = .0f;
	FPSCount = 0;
}

void accumulate()
{
	// 델타타임을 구한다.
	QueryPerformanceCounter(&NextCount);

	DeltaTime = ((float)(NextCount.QuadPart - CurCount.QuadPart)) / ((float)CpuFrequency.QuadPart);
	CurCount.QuadPart = NextCount.QuadPart;

	// FPS 계산 1초당 프레임 수...
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