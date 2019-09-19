#include "Player.h"

Player::Player() { }
Player::~Player() { }

char Player::Input(char ch, Map* map, NpcClass* npc)
{
	switch (ch)
	{
	case 72://위
		if (map->map[y - 1][x] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[--y][x] = DRAW_PLAYER;
		break;
	case 80://아래
		if (map->map[y + 1][x] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[++y][x] = DRAW_PLAYER;
		break;
	case 75://왼쪽
		if (map->map[y][x - 1] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[y][--x] = DRAW_PLAYER;
		break;
	case 77://오른쪽
		if (map->map[y][x + 1] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[y][++x] = DRAW_PLAYER;
		break;
	case 'z'://선택
		//앞에 몬스터, 아이템, NPC일 때
		if (map->map[y][x + 1] == DRAW_NPC || map->map[y][x - 1] == DRAW_NPC || map->map[y + 1][x] == DRAW_NPC || map->map[y - 1][x] == DRAW_NPC)
		{
			if(isReading)
				isReading = false;
			else
				isReading = true;
		}
		else if (map->map[y][x + 1] == DRAW_PORTAL || map->map[y][x - 1] == DRAW_PORTAL || map->map[y + 1][x] == DRAW_PORTAL || map->map[y - 1][x] == DRAW_PORTAL)
		{
			return DRAW_PORTAL;
			//포탈로 다른 맵으로 이동
			//여러가지 포탈에 대해 대처필요
		}
		else if (map->map[y][x + 1] == DRAW_MONSTER || map->map[y][x - 1] == DRAW_MONSTER || map->map[y + 1][x] == DRAW_MONSTER || map->map[y - 1][x] == DRAW_MONSTER)
		{
			return DRAW_MONSTER;
		}
		break;
	case 'x': //취소 (정보창 띄우기)
		break;
	}
	return 0;
}
void Player::Init(int _x, int _y, int _hp, int _mp, int _atk)
{
	x = _x;
	y = _y;
	hp = _hp;
	mp = _mp;
	atk = _atk;
	objectType = PLAYER;
}
void Player::Reading(NpcClass npc)
{
	if (isReading)
	{
		cout << npc.getScript();
	}
}