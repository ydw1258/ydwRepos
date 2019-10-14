#include "Bullet.h"
#include "Player.h"
#include "GameManager.h"

void Bullet::Init(int PlayerX, int PlayerY, DIRECTION _direction)
{
	direction = _direction;
	
	switch (direction)
	{
	case UP:
		x = PlayerX + 8;
		y = PlayerY;
		break;
	case DOWN:
		x = PlayerX + 8;
		y = PlayerY + 16;
		break;
	case LEFT:
		x = PlayerX;
		y = PlayerY + 8;
		break;
	case RIGHT:
		x = PlayerX + 16;
		y = PlayerY + 8;
		break;
	default:
		break;
	}
	sprite.Init(IMAGENUM_BULLET, 1, 8, 8, x, y);
}

void Bullet::Move(float deltaTime)
{
	speed = deltaTime * 300;

	switch (direction)
	{
	case UP:
		y -= speed;
		break;
	case DOWN:
		y += speed;
		break;
	case LEFT:
		x -= speed;
		break;
	case RIGHT:
		x += speed;
		break;
	default:
		break;
	}
}
bool Bullet::OutofRange()
{
	if (x < GameManager::GetInstance()->GameOffsetX || 
		x > TILEBLOCK_SIZE * TILE_WIDTH_NUM + GameManager::GetInstance()->GameOffsetX ||
		y < GameManager::GetInstance()->GameOffsetY ||
		y > TILEBLOCK_SIZE * TILE_HEIGHT_NUM + GameManager::GetInstance()->GameOffsetY)
		return true;
	return false;
}
void Bullet::Draw(HDC hdc)
{
	sprite.DrawObject(hdc, x, y);
}