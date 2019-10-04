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
	WORD roomIndex; //0�� �κ�
	char ID[10];
	char password[10];
};
struct ROOM_INFO
{
	int roomIndex; //0�� �κ�
	char ID[10]; //��û�� �÷��̾�
	int playerNum; //�� ���� �÷��̾� ��
	char ID[20][10];
};
//char chat[128];
struct PACKET_TRY_LOGIN
{
	PACKET_HEADER header;
	//�κ��� �÷��̾� ����
	ROOM_INFO roomInfo;
};
struct PACKET_TRY_ENTER_THE_ROOM
{
	PACKET_HEADER header;
	int userIndexInRoom;
	int roomNum;//�氹��
	ROOM_INFO roomInfo;
};
struct PACKET_GAMEEXIT
{
	PACKET_HEADER header;
	int roomNum; //�氹��
	int userIndexInRoom;
	ROOM_INFO roomInfo;
};
struct PACKET_TRY_EXIT_THE_ROOM //�������� EXIT�� �޾Ƽ� TRY ENTER THE ROOM��Ŷ���� ��ȯ
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
	char playerID[10]; //��û�� �÷��̾�
	int roomIndex;
	int userIndexInRoom;
};
#pragma pack()