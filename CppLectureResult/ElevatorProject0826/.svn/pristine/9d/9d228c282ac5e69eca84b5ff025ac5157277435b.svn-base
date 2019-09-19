#include "ElevatorManager.h"

void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

ElevatorManager::ElevatorManager(const int _ELEVATORNUM, const int _MAXFLOOR)
{
	elevator = new Elevator[_ELEVATORNUM];
	elevatorNum = _ELEVATORNUM;
	MAXFLOOR = _MAXFLOOR;
}
ElevatorManager::~ElevatorManager(){}

void ElevatorManager::ElevatorAllocation()
{
	AllocateStoppedElevator();
	AllocateMovedElevator();
}

void ElevatorManager::AllocateStoppedElevator()
{
	for (int i = 0; i < elevatorNum; i++)
	{
		//움직이지 않는 엘레베이터를 할당
		if (elevator[i].isMoving == false)
		{
			//전체 floor에서 버튼이 눌린 곳 찾음
			//전체 층의 절반 아래에 엘레베이터가 있다면 아래층 버튼부터 버튼 눌림확인
			if (elevator[i].curFloor < MAXFLOOR / 2)
			{
				for (int j = 0; j < MAXFLOOR; j++)
				{
					if (buttonStates[j].downButton == true && buttonStates[j].isReservedDownButton == false)
					{
						elevator[i].isMoving = true;
						elevator[i].StoppedElevatorCall(j, DOWN);
						buttonStates[j].isReservedDownButton = true;
						break;
					}
					else if (buttonStates[j].upButton == true && buttonStates[j].isReservedUpButton == false)
					{
						elevator[i].isMoving = true;
						elevator[i].StoppedElevatorCall(j, UP);
						buttonStates[j].isReservedUpButton = true;
						break;
					}
				}
			}
			else
			{
				for (int j = MAXFLOOR - 1; j >= 0; j--)
				{
					if (buttonStates[j].downButton == true && buttonStates[j].isReservedDownButton == false)
					{
						elevator[i].isMoving = true;
						elevator[i].StoppedElevatorCall(j, DOWN);
						buttonStates[j].isReservedDownButton = true;
						break;
					}
					else if (buttonStates[j].upButton == true && buttonStates[j].isReservedUpButton == false)
					{
						elevator[i].isMoving = true;
						elevator[i].StoppedElevatorCall(j, UP);
						buttonStates[j].isReservedUpButton = true;
						break;
					}
				}
			}
		}
	}
}
//가동중인 엘레베이터에 예약
void ElevatorManager::AllocateMovedElevator()
{
	for (int i = 0; i < elevatorNum; i++)
	{
		if (elevator[i].isStoppedElevatorCall == true)
			continue;

		for (int j = 0; j < MAXFLOOR; j++)
		{
			//진행방향 상에 있는 층의 예약만 받음
			if (elevator[i].targetDirection == UP && elevator[i].curFloor <= j &&
				buttonStates[j].upButton == true && buttonStates[j].isReservedUpButton == false)
			{
				elevator[i].ReservedFloors.push_back(j);
				sort(elevator[i].ReservedFloors.begin(), elevator[i].ReservedFloors.end(), greater<int>());
				buttonStates[j].isReservedUpButton = true;
				break;
			}
			else if (elevator[i].targetDirection == DOWN && elevator[i].curFloor >= j &&
				buttonStates[j].downButton == true && buttonStates[j].isReservedDownButton == false)
			{
				elevator[i].ReservedFloors.push_back(j);
				sort(elevator[i].ReservedFloors.begin(), elevator[i].ReservedFloors.end(), less<int>());
				buttonStates[j].isReservedDownButton = true;
				break;
			}
			//sort후 중복으로 저장된 층은 제거
			unique(elevator[i].ReservedFloors.begin(), elevator[i].ReservedFloors.end());
		}
	}
}

void ElevatorManager::MovingElevators(map<int, vector<People>>& mapWaitingPeopleNumInFloor)
{
	for (int i = 0; i < elevatorNum; i++)
	{
		bool isArrived = elevator[i].Move();
	
		if (isArrived)
		{
			if (elevator[i].targetDirection == UP)
			{
				buttonStates[elevator[i].curFloor].upButton = false;
				buttonStates[elevator[i].curFloor].isReservedUpButton = false;
			}
			else
			{
				buttonStates[elevator[i].curFloor].downButton = false;
				buttonStates[elevator[i].curFloor].isReservedDownButton = false;
			}
			elevator[i].Arrived(mapWaitingPeopleNumInFloor);
		}
	}
}
void ElevatorManager::Draw(map<int, vector<People>>& mapWaitingPeopleNumInFloor)
{
	for (int i = MAXFLOOR - 1; i >= 0; i--)
	{
		if(i < 9)
			cout << i + 1 << " 층 :  "; 
		else
			cout << i + 1 << " 층 : ";
		for (int j = 0; j < elevatorNum; j++)
		{
			if (elevator[j].curFloor == i)
			{
				if (elevator[j].ReservedFloors.empty())
				{
					textcolor(LIGHTBLUE, BLACK);
					cout << "[ " << elevator[j].peopleInElevator.size() << " ]";
					textcolor(WHITE, BLACK);
				}
				else if (elevator[j].isStoppedElevatorCall)
				{
					textcolor(YELLOW, BLACK);
					cout << "[ " << elevator[j].peopleInElevator.size() << " ]";
					textcolor(WHITE, BLACK);
				}
				else
				{
					textcolor(LIGHTGREEN, BLACK);
					cout << "[ " << elevator[j].peopleInElevator.size() << " ]";
					textcolor(WHITE, BLACK);
				}
			}
			else
				cout << ".   .";
		}

		if (buttonStates[i].upButton)
			cout << "▲";
		else
			cout << "△";
		if (buttonStates[i].downButton)
			cout << "▼ ";
		else
			cout << "▽ ";

		for (int j = 0; j < mapWaitingPeopleNumInFloor[i].size(); j++)
		{
			if (mapWaitingPeopleNumInFloor[i].at(j).direction == UP)
				cout << "▲";
			else
				cout << "▼";
		}

		cout << endl;
	}
	textcolor(LIGHTBLUE, BLACK);
	cout << "\t정지 ";
	textcolor(YELLOW, BLACK);
	cout << "예약 ";
	textcolor(LIGHTGREEN, BLACK);
	cout << "가동" << endl;
	textcolor(WHITE, BLACK);

	for (int j = 0; j < elevatorNum; j++)
	{
		cout << j + 1 << "번째 엘레베이터 ";
		if (elevator[j].ReservedFloors.empty())
		{
			cout << "현재 무게 : " << elevator[j].curElevatorWeight << " / " << elevator[j].maxAvailableWeight << "kg ";
			cout << "예약 안됨" << endl;
		}
		else if (elevator[j].isStoppedElevatorCall)
		{
			
			cout << "현재 무게 : " << elevator[j].curElevatorWeight << " / " << elevator[j].maxAvailableWeight << "kg ";
			cout << elevator[j].ReservedFloors.back() + 1 << " 층의 호출로 가는 중" << endl;
		}
		else
		{
			cout << "현재 무게 : " << elevator[j].curElevatorWeight  <<" / " << elevator[j].maxAvailableWeight << "kg ";
			cout << "예약된 층 : ";
			for (vector<int>::iterator it = elevator[j].ReservedFloors.begin(); 
				it != elevator[j].ReservedFloors.end(); it++)
			{
				cout << *it + 1 << " ";
			}
			
			cout << endl;
		}
	}
}