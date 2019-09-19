#include "NPC.h"

NpcClass::NpcClass()
{
	hp = 1;
	mp = 1;

}

void NpcClass::Init(int _x, int _y, string _script)
{
	x = _x;
	y = _y;
	script = _script;
	objectType = NPC;
}
NpcClass::~NpcClass()
{

}
