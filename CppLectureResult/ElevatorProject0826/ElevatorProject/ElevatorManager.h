#pragma once
#include"Elevator.h"
#include<vector>
#include<map>
#include<iostream>
#include<Windows.h>
using namespace std;

#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 

class ElevatorManager
{
private:
	int elevatorNum;
	int MAXFLOOR;
	Elevator* elevator;
public:
	map<int, ButtonState> buttonStates; //층, 버튼 눌림 상태
	ElevatorManager(const int ELEVATORNUM, const int MAXFLOOR);
	~ElevatorManager();
	void ElevatorAllocation();
	void AllocateStoppedElevator();
	void AllocateMovedElevator();
	void MovingElevators(map<int, vector<People>>& mapWaitingPeopleNumInFloor);
	void Draw(map<int, vector<People>>& mapWaitingPeopleNumInFloor);
};