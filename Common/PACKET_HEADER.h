#pragma once
#include <Windows.h>
#pragma pack(1)

enum Scene
{
	LOGIN,
	INGAME,
	LOBBY
};
enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET = 1,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_POS,
	PACKET_INDEX_SEND_CHATTING_INGAME,
	PACKET_INDEX_SEND_CHATTING_LOBBY,
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};
struct USER_DATA
{
	int playerNum;
	WORD wX;
	WORD wY;
	bool turn;
	char chat[128];
	int roomNum; //0Àº ·Îºñ
};
struct PACKET_LOGIN_RET
{
	PACKET_HEADER header;
	int iIndex;
};

struct PACKET_USER_DATA
{
	PACKET_HEADER header;
	WORD wCount;
	USER_DATA data[20];
};

struct PACKET_SEND_INGAME_DATA
{
	PACKET_HEADER header;
	USER_DATA data;
};

#pragma pack()