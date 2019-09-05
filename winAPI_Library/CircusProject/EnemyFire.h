#pragma once
#include"Windows.h"

enum FIRETYPE
{
	NORMAL,
	SMALL,
	FUTON
};
class EnemyFire
{
private:
	int x;
	int y;
	int moveSpeed;
	
	FIRETYPE fireType;
public:
	RECT FireRect;
	EnemyFire();
	~EnemyFire();
	void Init(FIRETYPE _fireType);
	void Move(int _moveSpeed);
	bool OutOfScreen();
	void Draw(HDC hdc);
	void RECTDisplay(HDC hdc);
};