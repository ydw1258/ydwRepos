#include "Bullet.h"
#include "Player.h"
#include "GameManager.h"

void Bullet::Init(int PlayerX, int PlayerY, DIRECTION _direction)
{
	direction = _direction;
	
	switch (direction)
	{
	case UP:
		x = PlayerX + 12;
		y = PlayerY;
		break;
	case DOWN:
		x = PlayerX + 12;
		y = PlayerY + 16;
		break;
	case LEFT:
		x = PlayerX;
		y = PlayerY + 12;
		break;
	case RIGHT:
		x = PlayerX + 16;
		y = PlayerY + 12;
		break;
	default:
		break;
	}
	//sprite.Init(IMAGENUM_BULLET, 1, 5, 5, x, y);
	sprite.Init(IMAGENUM_BULLET, 1, 8, 8, x, y);
}

void Bullet::Move()
{
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
	if (x < 20 || x > GameManager::GetInstance()->TileImageSizeX * 13 + 20 || y < 20 || y > GameManager::GetInstance()->TileImageSizeX * 13 + 20)
		return true;
	return false;
}
void Bullet::Draw(HDC hdc)
{
	sprite.DrawObject(hdc, x, y);
}