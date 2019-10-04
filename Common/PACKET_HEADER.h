#pragma once
#include <Windows.h>
#pragma pack(1)

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
	WORD playerNumInRoom;
	WORD wX;
	WORD wY;
	WORD roomIndex; //0은 로비
	char ID[10];
	char password[10];
};
struct ROOM_INFO
{
	int roomIndex; //0번 로비
	char ID[10]; //요청한 플레이어
	int playerNum; //방 내의 플레이어 수
	char ID[20][10];
};
//char chat[128];
struct PACKET_TRY_LOGIN
{
	PACKET_HEADER header;
	//로비의 플레이어 숫자
	ROOM_INFO roomInfo;
};
struct PACKET_TRY_ENTER_THE_ROOM
{
	PACKET_HEADER header;
	int userIndexInRoom;
	int roomNum;//방갯수
	ROOM_INFO roomInfo;
};
struct PACKET_GAMEEXIT
{
	PACKET_HEADER header;
	int roomNum; //방갯수
	int userIndexInRoom;
	ROOM_INFO roomInfo;
};
struct PACKET_TRY_EXIT_THE_ROOM //서버에서 EXIT를 받아서 TRY ENTER THE ROOM패킷으로 반환
{
	PACKET_HEADER header;
	int userIndexInRoom;
	ROOM_INFO roomInfo;
};
struct PACKET_LOGIN_RET
{
	PACKET_HEADER header;
};

struct PACKET_GAMESTART
{
	PACKET_HEADER header;
	char playerID[10]; //요청한 플레이어
	int roomIndex;
	int userIndexInRoom;
};
#pragma pack()