#pragma once;
#include<vector>
#include<map>
#include<algorithm>
#include<list>
#include"People.h"
using namespace std;

struct ButtonState
{
	bool upButton = false;
	bool downButton = false;
	bool isReservedUpButton = false;
	bool isReservedDownButton = false;
};

class Elevator
{
public:
	bool moveDirection = UP; //false는 UP true는 DOWN
	bool targetDirection = UP; //false는 UP true는 DOWN
	bool isMoving = false;
	bool isStoppedElevatorCall = false;
	int curFloor;
	int curElevatorWeight;
	int maxAvailableWeight;

	vector<int> ReservedFloors;
	list<People> peopleInElevator;
	Elevator(int _curFloor = 0, int _maxAvailableWeight = 500);
	void StoppedElevatorCall(int _targetFloor, bool peopleDirection);
	void OutElevator();
	void BoardandPushButtons(map<int, vector<People>>& mapWaitingPeopleNumInFloor);
	void Arrived(map<int, vector<People>>& mapWaitingPeopleNumInFloor);
	int Move();
};