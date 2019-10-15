#pragma once
#include <Windows.h>
#include <vector>
using namespace std;
#pragma pack(1)

#define BUFSIZE 1024
#define IDPWSIZE 16
#define CHATSIZE 128

enum PACKET_INDEX
{
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_TRY_LOGIN,
	PACKET_INDEX_LOGIN_RESULT,
	PACKET_INDEX_TRY_JOIN_ROOM,
	PACKET_INDEX_TRY_JOIN_ROOM_RESULT,
	PACKET_INDEX_JOIN_ROOM,
	PACKET_INDEX_CHAT,
	PACKET_INDEX_PLAYER_LEAVE_ROOM,
	PACKET_INDEX_ROOM_INFO_REQ,
	PACKET_INDEX_ROOM_INFO
};

enum PLAYER_STATE
{
	PS_LOGOUT,
	PS_IN_LOBBY,
	PS_PLAY,
	PS_SPECTATOR
};

enum LOGIN_RESULT
{
	LR_SUCCESS,
	LR_FAIL_ID_NOT_FOUND,
	LR_FAIL_WRONG_PW,
	LR_FAIL_ALREADY_LOGGED_IN,
	LR_FAIL_DATA_MISSING,
	LR_FAIL_MAX_USER
};

enum ROOM_JOIN_RESULT
{
	RJS_FAIL,
	RJS_SUCCESS,
	RJS_SPECTATOR
};

struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct USER_DATA
{
	PLAYER_STATE state;
	int roomNum;
	char name[IDPWSIZE];
};

//----

struct PACKET_USER_DATA
{
	PACKET_HEADER header;
	WORD wCount;
	USER_DATA data[20];
};

struct PACKET_TRY_LOGIN
{
	PACKET_HEADER header;
	char id[IDPWSIZE];
	char pw[64];
};

struct PACKET_LOGIN_RESULT
{
	PACKET_HEADER header;
	LOGIN_RESULT result;
};

struct PACKET_TRY_JOIN_ROOM
{
	PACKET_HEADER header;
	int roomNum;
};

struct PACKET_TRY_JOIN_ROOM_RESULT
{
	PACKET_HEADER header;
	int roomIdx;
	ROOM_JOIN_RESULT result;
};

struct PACKET_JOIN_ROOM
{
	PACKET_HEADER header;
	char userName[IDPWSIZE];
	int roomIdx;
	bool roomReady;
};

struct PACKET_CHAT
{
	PACKET_HEADER header;
	char msg[CHATSIZE];
};

struct PACKET_PLAYER_LEAVE_ROOM
{
	PACKET_HEADER header;
	char userName[IDPWSIZE];
	int roomIdx;
	bool destroyRoom;
};

struct PACKET_ROOM_INFO_REQ
{
	PACKET_HEADER header;
	int roomIdx;
};

struct PACKET_ROOM_INFO
{
	PACKET_HEADER header;
	int roomIdx;
	char playerIdW[IDPWSIZE];
	char playerIdB[IDPWSIZE];
};

#pragma pack()