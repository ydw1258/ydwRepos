#pragma once

enum DIRECTION
{
	STOP,
	LEFT,
	RIGHT,
};
class Player
{
private:
	bool isJump = false;

public:
	float x;
	float y;

	void Move(DIRECTION dir);
	void Attack();
	void Jump();
};

