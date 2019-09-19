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
	cout << "0 : �ڵ�, 1 : ����� ��� ";
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
				cout << "������ ��� �� : ";
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
		//���� ������ ���ݸ��� ��� ����
		elevatorManager.ElevatorAllocation();
		//���������� �̵�
		elevatorManager.MovingElevators(peopleManager.mapWaitingPeopleNumInFloor);
		count++;
		
		Sleep(200);
		system("cls");
		elevatorManager.Draw(peopleManager.mapWaitingPeopleNumInFloor);
	}
}

//������ ����� ����, ���� ����� ���� ���� (����ִ��� ���)
//���������� 4�� ���������Ͱ� ���� �ִ� ��, Ÿ�� �� �ο�, ����Ÿ�� �ִ� �ο�
//���������� ��ư (���չ�ư?)