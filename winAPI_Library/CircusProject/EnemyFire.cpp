#include "EnemyFire.h"
#include "ResourceManager.h"

EnemyFire::EnemyFire()
{
}

EnemyFire::~EnemyFire()
{
}

void EnemyFire::Init(FIRETYPE _fireType)
{
	fireType = _fireType;

	switch (fireType)
	{
	case NORMAL:
		x = 1200;
		y = 550;
		moveSpeed = 1;
		break;
	case SMALL:
		x = 1200;
		y = 550;
		moveSpeed = 1;
		break;
	case FUTON:
		x = 1200;
		y = 650;
		moveSpeed = 0;
		break;
	default:
		break;
	}
}

void EnemyFire::Draw(HDC hdc)
{
	switch (fireType)
	{
	case NORMAL:
		ResourceManager::GetInstance()->Draw(hdc, x, y, 25, 130, ENEMY_1b);
		ResourceManager::GetInstance()->Draw(hdc, x + 25, y, 25, 130, ENEMY_1f);

		//ResourceManager::GetInstance()->Draw(hdc, x, y, 25, 130, ENEMY_b);
		//ResourceManager::GetInstance()->Draw(hdc, x, y, 25, 130, ENEMY_f);
		break;
	case SMALL:
		ResourceManager::GetInstance()->Draw(hdc, x, y, 26, 104, ENEMY_l_b);
		ResourceManager::GetInstance()->Draw(hdc, x + 26, y, 25, 104, ENEMY_l_f);
		break;
	case FUTON:
		ResourceManager::GetInstance()->Draw(hdc, x, y, 50, 50, FRONT);
		//ResourceManager::GetInstance()->Draw(hdc, x, y, 50, 50, FRONT2);
		break;
	default:
		break;
	}
	
}
void EnemyFire::RECTDisplay(HDC hdc)
{
	Rectangle(hdc, FireRect.left, FireRect.top, FireRect.right, FireRect.bottom);
}
bool EnemyFire::OutOfScreen()
{
	if (x < 0)
		return true;
	return false;
}
void EnemyFire::Move(int _moveSpeed)
{
	x -= moveSpeed;
	x += _moveSpeed;

	switch (fireType)
	{
	case NORMAL:
		FireRect.left = x +5;
		FireRect.top = y + 100;
		FireRect.bottom = y + 130;
		FireRect.right = x + 10;
		break;
	case SMALL:
		FireRect.left = x + 5;
		FireRect.top = y + 90;
		FireRect.bottom = y + 100;
		FireRect.right = x + 10;
		break;
	case FUTON:
		FireRect.left = x + 10;
		FireRect.top = y;
		FireRect.bottom = y + 40;
		FireRect.right = x + 15;
		break;
	default:
		break;
	}
	
}