#include "PeopleManager.h"

PeopleManager::PeopleManager(){ }
PeopleManager::~PeopleManager(){ }

void PeopleManager::AutoGeneratePeople(int maxGeneratePeopleNum)
{
	int peopleRegenerationNum = rand() % maxGeneratePeopleNum + 1;
	People* peoples = new People[peopleRegenerationNum];
	
	for (int i = 0; i < peopleRegenerationNum; i++)
	{
		int peopleRegenerationCurFloor = rand() % 20;
		int peopleRegenerationTargetFloor = rand() % 20;

		while (peopleRegenerationCurFloor == peopleRegenerationTargetFloor)
			peopleRegenerationTargetFloor = rand() % 20;

		//������ ���� ���� 30~200kg
		int peopleWeight = rand() % 170 + 30; 

		peoples[i].Init(peopleRegenerationTargetFloor, peopleRegenerationCurFloor, peopleWeight);
		mapWaitingPeopleNumInFloor[peopleRegenerationCurFloor].push_back(peoples[i]);
	}
	delete[] peoples;
}
void PeopleManager::NonAutoGeneratePeoeple(int generatePeoepleNum)
{
	People* peoples = new People[generatePeoepleNum];

	for (int i = 0; i < generatePeoepleNum; i++)
	{
		int peopleRegenerationCurFloor;
		int peopleRegenerationTargetFloor;

		cout << i + 1 << "��° ��� ���� �� : ";
		cin >> peopleRegenerationCurFloor;
		peopleRegenerationCurFloor--;
		cout << i + 1 << "��° ��� ������ �� : ";
		cin >> peopleRegenerationTargetFloor;
		peopleRegenerationTargetFloor--;

		while (peopleRegenerationCurFloor == peopleRegenerationTargetFloor)
		{
			cout << "���� ��ġ�� ��ǥ��ġ�� ���� �� �����ϴ�." << endl;
			cout << i + 1 << "��° ��� ������ �� : ";
			cin >> peopleRegenerationTargetFloor;
		}
		int peopleWeight;
		cout << i + 1 << "��° ��� ������ : ";
		cin >> peopleWeight;

		peoples[i].Init(peopleRegenerationTargetFloor, peopleRegenerationCurFloor, peopleWeight);
		mapWaitingPeopleNumInFloor[peopleRegenerationCurFloor].push_back(peoples[i]);
	}

	delete[] peoples;
}

void PeopleManager::PushUpDownButton(const int MAXFLOOR, ElevatorManager& elevatorManager)
{
	for (int i = 0; i < MAXFLOOR; i++)
	{
		for (int j = 0; j < mapWaitingPeopleNumInFloor[i].size(); j++)
		{
			if (mapWaitingPeopleNumInFloor[i].at(j).direction == UP)
				elevatorManager.buttonStates[i].upButton = true;
			else
				elevatorManager.buttonStates[i].downButton = true;
		}
	}
}