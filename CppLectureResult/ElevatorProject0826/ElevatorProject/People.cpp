#include "People.h"

void People::Init(int _targetFloor, int _curFloor, int _weight)
{
	targetFloor = _targetFloor;
	weight = _weight;
	curFloor = _curFloor;

	if (targetFloor > curFloor)
		direction = UP;
	else
		direction = DOWN;
}