#include "PacketManager.h"
#include "ResourceManager.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include "SHA1.h"
#include <string>
#include<iostream>

using namespace std;
#define BUFSIZE 1024
#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

PacketManager* PacketManager::mthis = nullptr;

//여기 부터 서버구조 바뀌면 변경해야할 부분
void PacketManager::InitConnection()
{
	//서버한테 데이터 받기
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		return;
	}

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	int retval = connect(g_sock, (sockaddr*)& serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		exit(1);
	}

	retval = WSAAsyncSelect(g_sock, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		exit(1);
	}

	retval = WSAAsyncSelect(g_sock, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);

	if (retval == SOCKET_ERROR)
	{
		exit(1);
	}
}
void PacketManager::InputChatting()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char str[128];

	switch (scene)
	{
	case LOBBY:
	{
		PACKET_SEND_INGAME_DATA packet;

		GetWindowText(chatInputBoxLobby, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		packet.header.wIndex = PACKET_INDEX_SEND_CHATTING_LOBBY;
		packet.header.wLen = sizeof(packet);
		packet.data.playerNum = playerIndex;
		strcpy(packet.data.ID, playerID);
		strcpy(packet.data.chat, str);

		send(g_sock, (const char*)& packet, sizeof(packet), 0);
		SetWindowText(chatInputBoxLobby, "");
	}
	break;
	case ROOM_WAIT:
	{
		PACKET_SEND_INGAME_DATA packet;
		GetWindowText(chatInputBoxLobby, str, 128);
		GetWindowText(chatInputBoxIngame, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		packet.header.wIndex = PACKET_INDEX_SEND_CHATTING_INGAME;
		packet.header.wLen = sizeof(packet);
		packet.data.playerNum = playerIndex;
		strcpy(packet.data.ID, playerID);
		strcpy(packet.data.chat, str);

		send(g_sock, (const char*)& packet, sizeof(packet), 0);
		SetWindowText(chatInputBoxIngame, "");
	}
	break;
	}
}

void PacketManager::ClientExit(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;

	if (!Physics::GetInstance()->RECTbyPointCollisionCheck(RoomExitButton, pt))
	{
		return;
	}
	char buf[10];

	PACKET_GAMEEXIT packet;
	packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	packet.roomIndex = i + 1;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void PacketManager::EnterTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;
	bool flag = false;

	//i번째에 있는 방번호
	for (auto it = roomButtons.begin(); it != roomButtons.end(); it++, i++)
	{
		if (Physics::GetInstance()->RECTbyPointCollisionCheck(*it, pt))
		{
			flag = true;
			break;
		}
	}
	if (flag == false || mapRoomPlayers[i] == 4)
		return;
	char buf[10];

	PACKET_TRY_ENTER_THE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_ENTER_THE_ROOM;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	packet.roomIndex = i + 1;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void PacketManager::ExitTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;
	int i = 0;

	//i번째에 있는 방번호
	if (!Physics::GetInstance()->RECTbyPointCollisionCheck(roomExitButton, pt) || isGameStart)
	{
		return;
	}
	char buf[10];

	PACKET_TRY_EXIT_THE_ROOM packet;

	packet.header.wIndex = PACKET_INDEX_EXIT_THE_ROOM;
	packet.header.wLen = sizeof(packet);
	packet.roomIndex = roomIndex;

	strcpy(packet.playerID, playerID);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void PacketManager::GameStart(POINT pt)
{
	if (listPlayerID.size() < 2)
		return;
	/*if (!isGameStart)
	{
		isGameStart = true;
	}
	else
		return;*/

	if (Physics::GetInstance()->RECTbyPointCollisionCheck(startButton, pt))
	{
		if (userIndexInRoom != 0)
		{
			MessageBox(hwnd, "0번 유저만 시작 가능.", "?", MB_OK);
			return;
		}
		if (isGameStart)
		{
			return;
		}
		else
		{
			isGameStart = true;
		}
		//packet
		PACKET_GAMESTART packet;
		packet.header.wIndex = PACKET_INDEX_GAMESTART;
		packet.header.wLen = sizeof(packet);
		packet.userIndexInRoom = userIndexInRoom;
		packet.roomIndex = roomIndex;

		send(g_sock, (const char*)& packet, sizeof(packet), 0);
	}
}
void PacketManager::Login()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char buf[10];

	PACKET_TRY_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
	packet.header.wLen = sizeof(packet);

	GetWindowText(LOGINInput[0], buf, 10);
	strcpy(packet.ID, buf);
	GetWindowText(LOGINInput[1], buf, 10);
	char hash[11];
	strcpy(hash, sha1(buf).substr(0, 10).c_str());
	strcpy(packet.password, hash);

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendGameExit(int roomIndex)
{
	PACKET_GAMEEXIT packet;

	packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	packet.roomInfo.roomIndex = roomIndex;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendEnterTheRoomPacket(int roomIndex)
{
	PACKET_TRY_ENTER_THE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_ENTER_THE_ROOM;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	packet.roomInfo.roomIndex = roomIndex;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendExitTheRoom()
{
	PACKET_TRY_EXIT_THE_ROOM packet;

	packet.header.wIndex = PACKET_INDEX_EXIT_THE_ROOM;
	packet.header.wLen = sizeof(packet);
	packet.roomInfo.roomIndex = roomIndex;

	strcpy(packet.playerID, playerID);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendGameStart()
{
	PACKET_GAMESTART packet;
	packet.header.wIndex = PACKET_INDEX_GAMESTART;
	packet.header.wLen = sizeof(packet);
	packet.userIndexInRoom = userIndexInRoom;
	packet.roomInfo.roomIndex = roomIndex;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendLogin(char ID[10], char password[10])
{
	PACKET_TRY_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
	packet.header.wLen = sizeof(packet);

	strcpy(packet.ID, ID);
	
	char hash[11];
	strcpy(hash, sha1(password).substr(0, 10).c_str());
	strcpy(packet.password, hash);

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::SendPos(int x, int y)
{
	PACKET_SEND_INGAME_DATA packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.playerNum = playerIndex;
	packet.data.wX = x;
	packet.data.wY = y;
	packet.data.roomIndex = roomIndex;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void PacketManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int addrlen = 0;
	int retval = 0;

	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		//err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
			}
		}

		//구조체 선언
		USER_INFO_STRING userInfo = { 0 };

		while (true)
		{
			if (!ProcessPacket(szBuf, userInfo, retval, wParam))
			{
				Sleep(100);
				//SendMessage(`hWnd, uMsg, wParam, lParam);
				break;
			}
			else
			{
				if (userInfo.len < sizeof(PACKET_HEADER))
					break;
			}
		}
	}
	break;
	case FD_CLOSE:
		closesocket(wParam);
		exit(1);
		break;
	}
}
//서버로 부터 받은 데이터

//헤더 리턴 없는거
bool PacketManager::ProcessPacket(char* szBuf, USER_INFO_STRING& userinfo, int len, WPARAM wParam)
{
	if (len > 0)
	{
		memcpy(&userinfo.szBuf[userinfo.len], szBuf, len);
		userinfo.len += len;
		len = 0;
	}

	if (userinfo.len < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, userinfo.szBuf, sizeof(header));

	if (userinfo.len < header.wLen)
		return false;

	memcpy(&userinfo.szBuf, &userinfo.szBuf[header.wLen], userinfo.len - header.wLen);
	userinfo.len -= header.wLen;

	switch (header.wIndex)
	{
	case PACKET_INDEX_FIRST_CONNECT://첫 연결(클라이언트 실행)
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, szBuf, header.wLen);

		playerIndex = packet.iIndex;
		char temp[128];
		itoa(playerIndex, temp, 128);
	}
	break;
	case PACKET_INDEX_LOGIN_RET://로그인 시도
	{
		PACKET_TRY_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);

		//로그인 성공
		if (playerIndex != 99)
		{
			strcpy(playerID, packet.ID);
			playerIndex = packet.playerNum;
			userIndexInRoom = 0;
			SceneChange(LOBBY);
			GetPlayersInRoom(0);
		}
		else //로그인 실패
		{
			//MessageBox(, "로그인 실패", );
		}
	}
	break;
	case PACKET_INDEX_USER_DATA:
	{
		PACKET_USER_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
	}
	break;
	case PACKET_INDEX_SEND_POS:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		//gameovercheck
	}
	break;
	
	case PACKET_INDEX_GET_ROOMS:
	{
		PACKET_ROOMLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		for (int i = 0; i < packet.NumOfRoom; i++)
		{
			mapRoomPlayers[packet.roomIndex[i]] = packet.playerNum[i];
		}
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;
		memcpy(&packet, szBuf, header.wLen);
		isGameStart = true;
	}
	break;
	}

	return true;
}

//수정 요망
void func()
{
	//리턴 값이 필요한 헤더 함수
	switch (header.wIndex)
	{

		break;
	case PACKET_INDEX_SEND_CHATTING:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		chatList.push_back(packet.data.chat);
	}
	break;
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		listPlayerID.clear();
		listPlayerID.resize(0);

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.playerIDs[i]);
		}

		//로그인 패킷을 보낸 유저만 서버로 발송
		if (!strcmp(packet.userID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		if (!packet.isSuccess)
			return true;

		roomIndex = packet.roomInfo.roomIndex;
		listPlayerID.clear();

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.ID[i]);
		}

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			SceneChange(ROOM_WAIT);
		}
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_GAMEEXIT:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		listPlayerID.clear();
		listPlayerID.resize(0);
		strcpy(packet.userID, playerID);
		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.playerIDs[i]);
		}
		closesocket(wParam);
	}
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		if (!packet.isSuccess)
			return true;

		roomIndex = packet.roomInfo.roomIndex;
		listPlayerID.clear();

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.ID[i]);
		}

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			SceneChange(ROOM_WAIT);
		}
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_EXIT_THE_ROOM:
	{
		PACKET_TRY_EXIT_THE_ROOM packet;

		memcpy(&packet, szBuf, header.wLen);

		roomIndex = packet.roomIndex;
		listPlayerID.clear();

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.ID[i]);
		}
		userIndexInRoom = 0;

		//fill_n(BoardInfo, HEIGHT * WIDTH, 0);

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			SceneChange(LOBBY);
		}
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_SEND_CHATTING:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		chatList.push_back(packet.data.chat);
	}
	break;
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		listPlayerID.clear();
		listPlayerID.resize(0);

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.playerIDs[i]);
		}

		//로그인 패킷을 보낸 유저만 서버로 발송
		if (!strcmp(packet.userID, playerID))
			GetRooms();
	}
	break;
	}
}

void PacketManager::GetPlayersInRoom(int roomNum)
{
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GET_PLAYERS;
	packet.roomInfo.roomIndex = roomNum;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void PacketManager::GetRooms()
{
	PACKET_ROOMLIST packet;

	packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
	packet.header.wLen = sizeof(packet);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
PacketManager::PacketManager() {}

PacketManager::~PacketManager()
{
	closesocket(g_sock);
	WSACleanup();
}