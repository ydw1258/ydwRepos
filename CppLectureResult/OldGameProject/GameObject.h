#pragma once
enum OBJECTTYPE
{
	PLAYER,
	MONSTER,
	NPC,
	PORTAL
};
class GameObject
{
public:
	OBJECTTYPE objectType;
	int hp;
	int mp;
	int x;
	int y;
	
	GameObject();
	~GameObject();
};

