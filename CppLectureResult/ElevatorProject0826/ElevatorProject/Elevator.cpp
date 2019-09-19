#include "Elevator.h"
#include<iostream>

Elevator::Elevator(int _curFloor, int _maxAvailableWeight)
{
	curFloor = _curFloor;
	maxAvailableWeight = _maxAvailableWeight;
	curElevatorWeight = 0;
}

int Elevator::Move()
{
	if (ReservedFloors.empty() == false && ReservedFloors.back() == curFloor) //���� ����� �������� ����
		return 1; //ElevatorMaanger ���� Arrived ȣ��
	if (isMoving == false)
		return 0;
	if (moveDirection == UP)
		curFloor++;
	else
		curFloor--;
	return 0;
}

void Elevator::Arrived(map<int, vector<People>>& mapWaitingPeopleNumInFloor)
{
	ReservedFloors.pop_back();

	if (isStoppedElevatorCall == true)
	{
		isStoppedElevatorCall = false;
		BoardandPushButtons(mapWaitingPeopleNumInFloor);
		moveDirection = targetDirection;
		
		return;
	}
	OutElevator();
	BoardandPushButtons(mapWaitingPeopleNumInFloor);
}
void Elevator::OutElevator()
{
	//Ÿ���ִ� ��� ����
	for (list<People>::iterator iter = peopleInElevator.begin(); iter != peopleInElevator.end();)
	{
		if (iter->targetFloor == curFloor)
		{
			curElevatorWeight -= iter->weight;

			iter = peopleInElevator.erase(iter);
		}
		else
			++iter;
	}
}
void Elevator::BoardandPushButtons(map<int, vector<People>>& mapWaitingPeopleNumInFloor)
{
	//ž��, ��ư����
	//���� Ÿ�ٰ� ����� ��ư�� ��, ���� ����� Ÿ������ �缳��
	//���� ����� ��ư�� ���ٸ� ���� ����
	for (vector<People>::iterator iter = mapWaitingPeopleNumInFloor[curFloor].begin(); iter != mapWaitingPeopleNumInFloor[curFloor].end();)
	{
		//�����ʰ�
		if (iter->weight + curElevatorWeight > maxAvailableWeight)
			break;

		if (iter->direction == targetDirection)
		{
			//���� ���������Ϳ� �߰�
			peopleInElevator.push_back(*iter);
			curElevatorWeight += iter->weight;
			//�°��� ��ȣ ������
			ReservedFloors.push_back(iter->targetFloor);
			//�ش� �� �ο� ����
			iter = mapWaitingPeopleNumInFloor[curFloor].erase(iter);
		}
		else
		{
			++iter;
		}
	}
	if (targetDirection == UP)
		sort(ReservedFloors.begin(), ReservedFloors.end(), greater<int>());
	else
		sort(ReservedFloors.begin(), ReservedFloors.end(), less<int>());
	if (ReservedFloors.empty() == true)
		isMoving = false;
}

//�񰡵����� ���������� �Ҵ�
void Elevator::StoppedElevatorCall(int _targetFloor, bool peopleDirection)
{
	isStoppedElevatorCall = true;
	isMoving = true;
	ReservedFloors.push_back(_targetFloor);
	targetDirection = peopleDirection;

	if (curFloor < _targetFloor)
		moveDirection = UP;
	else
		moveDirection = DOWN;
}