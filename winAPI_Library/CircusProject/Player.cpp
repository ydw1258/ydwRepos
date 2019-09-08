#include "Player.h"
#include "GameManager.h"

RECT Player::GetPlayerRect()
{
	RECT playerRect;
	playerRect.top = y;
	playerRect.bottom = y + 40;
	playerRect.left = x + 20;
	playerRect.right = x + 40;
	return playerRect;
}

void Player::Init()
{
	moveSprite[0].Init(PLAYER0, 1, 66, 63);
	moveSprite[1].Init(PLAYER1, 1, 66, 63);
	jumpSprite.Init(PLAYER2, 1, 66, 63);
	dieSprite.Init(IMAGENUM_DIE, 1, 66, 63);
}

void Player::Move(bool direction)
{
	if (isJump)
		return;
	if (direction)
	{
		playerState = RIGHT;
	}
	else
	{
		playerState = LEFT;
	}
}
void Player::Jump()
{
	if (!isJump)
		return;
	curSprite = PLAYER2;

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
			curSprite = PLAYER0;
		}
	}
}
void Player::Stop()
{
	playerState = STOP;
}
void Player::Draw(HDC hdc)
{
	switch (playerState)
	{
	case STOP:
	case LEFT:
	case RIGHT:
		moveSprite[curSprite - PLAYER0].DrawObject(hdc, x + GameManager::GetInstance()->CameraX, y);
		//moveSprite[1].DrawObject(hdc, x, y);
		break;
	case PLAYER_DIE:
		dieSprite.DrawObject(hdc, x, y);
		break;
	default:
		break;
	}
	
}
void Player::SpriteChange()
{
	if (isJump)
		return;

	switch (playerState)
	{
	case STOP:
		break;
	case LEFT:
	case RIGHT:
		if (curSprite == PLAYER0)
			curSprite = PLAYER1;
		else
			curSprite = PLAYER0;
		break;
	case PLAYER_DIE:
		break;
	default:
		break;
	}
}

Player::Player(){}
Player::~Player(){}