#include "PeopleManager.h"

PeopleManager::PeopleManager(const int MAXFLOOR)
{
}

PeopleManager::~PeopleManager()
{
}
void PeopleManager::GeneratePeople(int maxGeneratePeopleNum)
{
	int peopleRegenerationNum = rand() % maxGeneratePeopleNum + 1;
	People* people = new People[peopleRegenerationNum];
	
	for (int i = 0; i < peopleRegenerationNum; i++)
	{
		int peopleRegenerationCurFloor = rand() % 20;
		int peopleRegenerationTargetFloor = rand() % 20;

		while (peopleRegenerationCurFloor == peopleRegenerationTargetFloor)
			peopleRegenerationTargetFloor = rand() % 20;

		int peopleWeight = rand() % 200 + 30;

		people[i].Init(peopleRegenerationTargetFloor, peopleRegenerationCurFloor, peopleWeight);
		mapWaitingPeopleNumInFloor[peopleRegenerationCurFloor].push_back(people[i]);
		
	}
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