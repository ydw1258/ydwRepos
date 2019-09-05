#include "Player.h"

void Player::Move(bool direction)
{
	if (isJump)
		return;
	playerRECT.left = x + 10;
	playerRECT.top = y;
	playerRECT.right = x + 30;
	playerRECT.bottom = y + 20;

	if (direction)
	{
		playerState = LEFT;
		if(playerImage == PLAYER1)
			playerImage = PLAYER0;
		else if (playerImage == PLAYER0)
			playerImage = PLAYER1;
	}
	else
	{
		playerState = RIGHT;
		if (playerImage == PLAYER1)
			playerImage = PLAYER0;
		else if (playerImage == PLAYER0)
			playerImage = PLAYER1;
	}
}

void Player::Jump()
{
	if (!isJump)
		return;
	playerRECT.left = x + 10;
	playerRECT.top = y;
	playerRECT.right = x + 30;
	playerRECT.bottom = y + 20;

	if (isUp)
	{
		y -= jumpspeed;
		if (y < jumpTopY)
		{
			isUp = false;
		}
	}
	else
	{
		y+= jumpspeed;
		
		if (y == defaultY)
		{
			
			isJump = false;
			isUp = true;
			playerImage = PLAYER0;
		}
	}
}
void Player::Stop()
{
	playerState = STOP;
}
void Player::Draw(HDC hdc)
{
	ResourceManager::GetInstance()->Draw(hdc, x, y, 66, 63, playerImage);
	
}
void Player::RECTDisplay(HDC hdc)
{
	Rectangle(hdc, playerRECT.left, playerRECT.top, playerRECT.right, playerRECT.bottom);
}

Player::Player()
{
	
}

Player::~Player()
{
}
