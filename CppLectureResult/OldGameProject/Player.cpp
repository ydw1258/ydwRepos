#include "Player.h"

Player::Player() { }
Player::~Player() { }

char Player::Input(char ch, Map* map, NpcClass* npc)
{
	switch (ch)
	{
	case 72://��
		if (map->map[y - 1][x] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[--y][x] = DRAW_PLAYER;
		break;
	case 80://�Ʒ�
		if (map->map[y + 1][x] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[++y][x] = DRAW_PLAYER;
		break;
	case 75://����
		if (map->map[y][x - 1] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[y][--x] = DRAW_PLAYER;
		break;
	case 77://������
		if (map->map[y][x + 1] != DRAW_EMPTY || isReading)
			return 0;
		map->map[y][x] = DRAW_EMPTY;
		map->map[y][++x] = DRAW_PLAYER;
		break;
	case 'z'://����
		//�տ� ����, ������, NPC�� ��
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
			//��Ż�� �ٸ� ������ �̵�
			//�������� ��Ż�� ���� ��ó�ʿ�
		}
		else if (map->map[y][x + 1] == DRAW_MONSTER || map->map[y][x - 1] == DRAW_MONSTER || map->map[y + 1][x] == DRAW_MONSTER || map->map[y - 1][x] == DRAW_MONSTER)
		{
			return DRAW_MONSTER;
		}
		break;
	case 'x': //��� (����â ����)
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