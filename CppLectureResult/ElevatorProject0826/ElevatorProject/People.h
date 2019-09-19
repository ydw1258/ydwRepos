#pragma once;
#define UP 0
#define DOWN 1

class People
{
public:
	int targetFloor;
	int curFloor;
	int weight;
	bool direction;
	void Init(int _targetFloor, int _curFloor, int _weight);
};