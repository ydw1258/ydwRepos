#pragma once
#pragma pack(1)

#define SERVERPORT 9000
#define BUFSIZE 1024
#define WM_SOCKET (WM_USER+1)
#define ROOMNUM 20
#define MAX_PLAYERNUM_INROOM 4

#include <Windows.h>
#pragma comment(lib, "ws2_32")

enum SERVERMANAGER_SCENE
{
	LOGIN,
	LOBBY,
	ROOM_WAIT,
	PLAYING,
};
enum PACKET_INDEX
{
	PACKET_INDEX_FIRST_CONNECT = 1,
	PACKET_INDEX_LOGIN_RET,
	PACKET_INDEX_USER_DATA,
	PACKET_INDEX_SEND_POS,
	PACKET_INDEX_GET_PLAYER_INLOBBY,
	PACKET_INDEX_SEND_CHATTING,
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
	WORD playerNum;
	WORD wX;
	WORD wY;
	char chat[128];
	WORD roomIndex; //0은 로비
	char ID[10];
	char password[10];
	int userIndexInRoom;
};
class USER_INFO
{
public:
	WORD playerIndexInRoom;
	WORD roomIndex;
	char playerID[10];
	char szBuf[BUFSIZE];
	int len;
};
class ROOM_INFO
{
public:
	int roomIndex;
	char IDs[20][10]; //로비 최대 수용인원 20, 방은 4로
	int playerNum;
};
struct PACKET_TRY_LOGIN
{
	PACKET_HEADER header;
	WORD playerIndex;
	WORD playerNum;
	char ID[10];
	char password[11];
};
struct PACKET_TRY_ENTER_THE_ROOM
{
	PACKET_HEADER header;
	ROOM_INFO roomInfo;
	char playerID[10];
	int userIndexInRoom;
};
struct PACKET_GAMEEXIT
{
	PACKET_HEADER header;
	ROOM_INFO roomInfo;
	char playerID[10];
	int userIndexInRoom;
};
struct PACKET_TRY_EXIT_THE_ROOM //서버에서 EXIT를 받아서 TRY ENTER THE ROOM패킷으로 반환
{
	PACKET_HEADER header;
	ROOM_INFO roomInfo;
	char playerID[10]; //요청한 플레이어
	int userIndexInRoom;
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
	SERVERMANAGER_SCENE scene;
};
struct PACKET_USERSLIST
{
	PACKET_HEADER header;
	ROOM_INFO roomInfo;
	char playerID[10];
};
struct PACKET_ROOMLIST //여러번 보내는 걸로 변경 예정.
{
	PACKET_HEADER header;
	int NumOfRoom;
	int roomIndex[ROOMNUM];
	int playerNum[ROOMNUM];
	bool isPlaying[ROOMNUM];
};
struct PACKET_GAMESTART
{
	PACKET_HEADER header;
	ROOM_INFO roomInfo;
	char playerID[10]; //요청한 플레이어
	int userIndexInRoom;
};
#pragma pack()