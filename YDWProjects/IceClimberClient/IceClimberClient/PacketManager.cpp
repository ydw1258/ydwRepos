#include "PacketManager.h"
#include "IceClimberGameManager.h"

using namespace std;
#define BUFSIZE 1024
#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

PacketManager* PacketManager::mthis = nullptr;

//여기 부터 서버구조 바뀌면 변경해야할 부분
void PacketManager::InitConnection(HWND hwnd)
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
/*
void PacketManager::ClientExit(POINT pt)//게임 종료 버튼 (보류)
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
*/
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

void PacketManager::SendLogin(char* ID, char* password)
{
	PACKET_TRY_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
	packet.header.wLen = sizeof(packet);
	
	strcpy(packet.ID, ID);
	
	char hash[11];
	strcpy(hash, sha1(password).substr(0, 10).c_str());
	strcpy(packet.password, hash);
	packet.playerNum = 99;
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
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

void PacketManager::SendChattingData(char * str)
{
	PACKET_SEND_INGAME_DATA packet;

	packet.header.wIndex = PACKET_INDEX_SEND_CHATTING;
	packet.header.wLen = sizeof(packet);
	packet.data.playerNum = playerIndex;
	strcpy(packet.data.ID, playerID);
	strcpy(packet.data.chat, str);

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

		PacketManager::GetInstance()->ProcessPacket(szBuf, retval, wParam);
	}
	break;
	case FD_CLOSE:
		closesocket(wParam);
		exit(1);
		break;
	}
}
//서버로 부터 받은 데이터
bool PacketManager::ProcessPacket(char* szBuf, int len, WPARAM wParam)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;

	cout << "패킷받음." << len << endl;

	// 짧음
	if (myLen < sizeof(PACKET_HEADER))
	{
		cout << "패킷받음. 헤더보다 짧음" << endl;
		return false;
	}

	PACKET_HEADER header;
	memcpy(&header, packetBuf, sizeof(header));

	if (myLen < header.wLen)
	{
		cout << "패킷받음. header.wLen보다 짧음" << endl;
		return false;
	}

	// 긺
	if (myLen > header.wLen)
	{
		cout << "패킷받음. header.wLen보다 김." << endl;
		cout << "myLen: " << myLen << " / header.wLen: " << header.wLen
			<< "header.wIndex: " << header.wIndex << endl;
	}

	//
	
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
		GameManager::GetInstance()->SceneChange(PLAYING);
	}
	break;
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_TRY_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);
		
		//로그인 성공
		if (packet.isSuccess)
		{
			strcpy(playerID, packet.ID);
			userIndexInRoom = packet.playerIndexInroom;
			GetPlayersInRoom(0);
			GameManager::GetInstance()->SceneChange(LOBBY);
		}
		else //로그인 실패
		{
			MessageBox(hwnd, "고오","로그인 실패", MB_OK);
		}
	}
	break;
	case PACKET_INDEX_SEND_CHATTING:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		GameManager::GetInstance()->chatList.push_back(packet.data.chat);
	}
	break;
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		GameManager::GetInstance()->listPlayerID.clear();
		GameManager::GetInstance()->listPlayerID.resize(0);

		for (int i = 0; i < packet.roomInfo.playerNum; i++)
		{
			GameManager::GetInstance()->listPlayerID.push_back(packet.roomInfo.IDs[i]);
		}

		//로그인 패킷을 보낸 유저만 서버로 발송
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	
	case PACKET_INDEX_GAMEEXIT:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		GameManager::GetInstance()->listPlayerID.clear();
		GameManager::GetInstance()->listPlayerID.resize(0);
		strcpy(packet.playerID, playerID);
		for (int i = 0; i < packet.roomInfo.playerNum; i++)
		{
			GameManager::GetInstance()->listPlayerID.push_back(packet.roomInfo.IDs[i]);
		}
		closesocket(wParam);
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		roomIndex = packet.roomInfo.roomIndex;
		GameManager::GetInstance()->listPlayerID.clear();

		for (int i = 0; i < packet.roomInfo.playerNum; i++)
		{
			GameManager::GetInstance()->listPlayerID.push_back(packet.roomInfo.IDs[i]);
		}

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			GameManager::GetInstance()->SceneChange(ROOM_WAIT);
		}
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_EXIT_THE_ROOM:
	{
		PACKET_TRY_EXIT_THE_ROOM packet;

		memcpy(&packet, szBuf, header.wLen);

		roomIndex = packet.roomInfo.roomIndex;
		GameManager::GetInstance()->listPlayerID.clear();

		for (int i = 0; i < packet.roomInfo.playerNum; i++)
		{
			GameManager::GetInstance()->listPlayerID.push_back(packet.roomInfo.IDs[i]);
		}
		userIndexInRoom = 0;

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			GameManager::GetInstance()->SceneChange(LOBBY);
		}
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	}

	return true;
}

void PacketManager::GetPlayersInRoom(int roomIndex)
{
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GET_PLAYERS;
	packet.roomInfo.roomIndex = 0;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
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