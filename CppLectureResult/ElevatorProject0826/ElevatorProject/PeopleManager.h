#pragma once
#include"People.h"
#include"ElevatorManager.h"
#include<vector>
#include<map>
using namespace std;

class PeopleManager
{
private:
public:
	map<int, vector<People>> mapWaitingPeopleNumInFloor;
	PeopleManager();
	~PeopleManager();

	void AutoGeneratePeople(int maxGeneratePeopleNum);
	void NonAutoGeneratePeoeple(int generatePeoepleNum);
	void PushUpDownButton(const int MAXFLOOR, ElevatorManager& elevatorManager);
};