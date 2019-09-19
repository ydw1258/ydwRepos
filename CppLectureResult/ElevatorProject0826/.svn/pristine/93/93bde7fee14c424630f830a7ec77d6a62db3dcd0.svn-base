#include<iostream>
#include<ctime>
#include<Windows.h>
#include<map>
#include<vector>
#include<cmath>
#include<conio.h>

#include"People.h"
#include"Elevator.h"
#include"ElevatorManager.h"
#include"PeopleManager.h"

using namespace std;

int main()
{
	const int ELEVATORNUM = 4;
	const int MAXFLOOR = 20;
	const int MAXPEOPLEREGENERATIONNUM = 5;

	ElevatorManager elevatorManager(ELEVATORNUM, MAXFLOOR);
	PeopleManager peopleManager;

	srand((unsigned int)time(0));
	
	const int PEOPLEREGENERATIONTERM = 10;
	int count = PEOPLEREGENERATIONTERM;

	bool sw;
	cout << "0 : 자동, 1 : 디버그 모드 ";
	cin >> sw;
	bool flag = false;

	while (true)
	{
		switch (sw)
		{
		case 0:
			if (count == PEOPLEREGENERATIONTERM)
			{
				count = 0;
				peopleManager.AutoGeneratePeople(MAXPEOPLEREGENERATIONNUM);
				peopleManager.PushUpDownButton(MAXFLOOR, elevatorManager);
			}
			break;
		default:
			int n;
			if (!flag)
			{
				cout << "생성할 사람 수 : ";
				cin >> n;
				peopleManager.NonAutoGeneratePeoeple(n);
				peopleManager.PushUpDownButton(MAXFLOOR, elevatorManager);
				flag = true;
			}
			else
			{
				if (count >= 1)
				{
					peopleManager.PushUpDownButton(MAXFLOOR, elevatorManager);
					getchar();
					count = 0;
				}
			}
				
			break;
		}
		//일정 프레임 간격마다 사람 생성
		elevatorManager.ElevatorAllocation();
		//엘레베이터 이동
		elevatorManager.MovingElevators(peopleManager.mapWaitingPeopleNumInFloor);
		count++;
		
		Sleep(200);
		system("cls");
		elevatorManager.Draw(peopleManager.mapWaitingPeopleNumInFloor);
	}
}

//층마다 사람이 랜덤, 층에 사람을 직접 넣음 (몇명있는지 출력)
//엘레베이터 4개 엘레베이터가 현재 있는 층, 타는 총 인원, 현재타고 있는 인원
//엘레베이터 버튼 (통합버튼?)