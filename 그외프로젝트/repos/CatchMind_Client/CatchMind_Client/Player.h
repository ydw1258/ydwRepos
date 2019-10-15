#pragma once
#include "../../CatchMind/CatchMind/PACKET.h"

class Player
{
public:
	char id[IDPWSIZE];
	PLAYER_STATE myState = PS_IN_LOBBY;

	Player();
	~Player();
};

