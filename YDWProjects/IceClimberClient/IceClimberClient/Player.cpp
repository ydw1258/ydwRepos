#include "Player.h"

void Player::Init()
{


	CurSprite = rightStop;
}

void Player::Move(DIRECTION dir)
{
	switch (dir)
	{
	case STOP:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	}
}

void Player::Attack()
{
}

void Player::Jump(DIRECTION dir)
{
	switch (dir)
	{
	case STOP: 
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	default:
		break;
	}
}