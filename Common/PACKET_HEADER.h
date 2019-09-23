#pragma once
#include <Windows.h>
#include<string>
using namespace std;
#pragma pack(1)

enum Scene
{
	LOGIN,
	LOBBY,
	INGAME,
};

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET = 1,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_POS,
	PACKET_INDEX_SEND_CHATTING_INGAME
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct USER_DATA
{
	int playerNum;
	Scene playerScene;
	WORD wX;
	WORD wY;
	bool turn;
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

struct PACKET_SEND_POS
{
	PACKET_HEADER header;
	USER_DATA data;
};

struct PACKET_SEND_CHAT_MESSAGE
{
	PACKET_HEADER header;
	char chatMessage[128];
	USER_DATA data;
};
#pragma pack()