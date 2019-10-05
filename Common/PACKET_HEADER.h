#pragma once
#include <Windows.h>
#pragma pack(1)

enum Scene
{
	LOGIN,
	LOBBY,
	INGAME,
	
};
enum PACKET_INDEX
{
	PACKET_INDEX_FIRST_CONNECT = 1,
	PACKET_INDEX_LOGIN_RET,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_POS,
	PACKET_INDEX_GET_PLAYER_INLOBBY,
	PACKET_INDEX_SEND_CHATTING_LOBBY,
	PACKET_INDEX_SEND_CHATTING_INGAME,
	PACKET_INDEX_GET_PLAYERS,
	PACKET_INDEX_GET_ROOMS,
	PACKET_INDEX_ENTER_THE_ROOM,
	PACKET_INDEX_EXIT_THE_ROOM,
	PACKET_INDEX_GAMESTART,
	PACKET_INDEX_GAMEEXIT,
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
	int roomIndex; //0은 로비
	char ID[128];
	char password[128];
};
struct PACKET_TRY_LOGIN
{
	PACKET_HEADER header;
	int playerNum;
	char ID[128];
	char password[128];
	bool isLoginSuccess = false;
	int roomIndex;
};
struct PACKET_TRY_ENTER_THE_ROOM
{
	PACKET_HEADER header;
	int roomNum; //방갯수
	int userIndexInRoom;
	char playerID[128];
	char ID[4][128];
	int roomIndex;
	bool isSuccess = false;
	int playerNum;
};
struct PACKET_GAMEEXIT
{
	PACKET_HEADER header;
	int roomNum; //방갯수
	int userIndexInRoom;
	char playerID[128];
	char ID[4][128];
	int roomIndex;
	bool isSuccess = false;
	int playerNum;
};
struct PACKET_TRY_EXIT_THE_ROOM //서버에서 EXIT를 받아서 TRY ENTER THE ROOM패킷으로 반환
{
	PACKET_HEADER header;
	int roomIndex;
	char playerID[128]; //요청한 플레이어
	char ID[4][128];
	int userIndexInRoom;
	bool isSuccess = false;
	int playerNum;
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
	Scene scene;
};
struct PACKET_USERSLIST
{
	PACKET_HEADER header;
	int playerNum;
	int roomIndex;
	char playerIDs[5][10];
	char userID[10];
};
struct PACKET_ROOMLIST //여러번 보내는 걸로 변경 예정.
{
	PACKET_HEADER header;
	int NumOfRoom;
	int roomIndex[10];
	int playerNum[10];
	bool isPlaying[10];
	char roomName[128];
};
struct PACKET_GAMESTART
{
	PACKET_HEADER header;
	char playerID[128]; //요청한 플레이어
	bool MyStone; //0흑 1백
	int roomIndex;
	int userIndexInRoom;
};
#pragma pack()