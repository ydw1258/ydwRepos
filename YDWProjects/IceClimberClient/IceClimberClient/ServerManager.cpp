#include "ServerManager.h"
#include "ResourceManager.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include "SHA1.h"
#include <string>
#include <Windows.h>
#include<iostream>

using namespace std;
#define BUFSIZE 1024
#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

ServerManager* ServerManager::mthis = nullptr;

void ServerManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[7] = { "Resources\\board.bmp"
	};
	ResourceManager::GetInstance()->Init(hdc, hInstance, filename, 7);

	board.Init(IMAGENUM_BOARD, 1, 722, 720);

	InitConnection();
	SceneInitiator();
}
void ServerManager::SceneInitiator()
{
	switch (scene)
	{
	case LOGIN:
		LOGINInput[0] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 300, 300, 300, 30, hwnd, (HMENU)0, hInstance, NULL);
		LOGINInput[1] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 300, 350, 300, 30, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case LOBBY:
		chatInputBoxLobby = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 750, 800, 30, hwnd, (HMENU)0, hInstance, NULL);
		chatList.clear();
		//chatInputBox = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case ROOM_WAIT:
		chatInputBoxIngame = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		chatList.clear();
		break;
	}
}
void ServerManager::Draw(HDC hdc)
{
	switch (scene)
	{
	case LOGIN:
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		break;
	case LOBBY:
	{
		//Draw UI, Images
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		memoImage.DrawResizedObject(hdc, 650, 10, 300, 400);
		blueBoard.DrawResizedObject(hdc, 40, 520, 700, 200);
		blueBoard.DrawResizedObject(hdc, gameExitButton.left, gameExitButton.top, gameExitButton.right - gameExitButton.left
			, gameExitButton.bottom - gameExitButton.top);

		buttonFont.Draw("���� ����", 20, 780, 570, "Resources/oldgameFont.ttf", RGB(255, 0, 0));
		//Rectangle(hdc, gameExitButton.left, gameExitButton.top, gameExitButton.right, gameExitButton.bottom);
		DrawChatWindow(hdc);
		DrawRooms(hdc);
		DrawCurUsers(hdc);
	}
	break;
	case ROOM_WAIT:
	{
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		//font.Draw(playerID, 30, 800, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));

		DrawCurUsers(hdc);
		DrawChatWindow(hdc);

		char buf[128];
		sprintf(buf, "%d ���� %d��° ����", roomIndex, userIndexInRoom);
		playerInfoFont.Draw(buf, 30, 720, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));
		blueBoard.DrawResizedObject(hdc, RoomExitButton.left, RoomExitButton.top, RoomExitButton.right - RoomExitButton.left
			, RoomExitButton.bottom - RoomExitButton.top);
		roomexitFont.Draw("������", 20, 550, 730, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		blueBoard.DrawResizedObject(hdc, startButton.left, startButton.top, startButton.right - startButton.left
			, startButton.bottom - startButton.top);
		gamestartFont.Draw("���� ����", 20, 350, 730, "Resources/oldgameFont.ttf", RGB(0, 0, 0));

		//Rectangle(hdc, startButton.left, startButton.top, startButton.right, startButton.bottom);
		//Rectangle(hdc, gameExitButton.left, gameExitButton.top, gameExitButton.right, gameExitButton.bottom);
	}
	break;
	}
}

void ServerManager::SceneChange(SERVERMANAGER_SCENE _scene)
{
	switch (scene)
	{
	case LOGIN:
		DestroyWindow(LOGINInput[0]);
		DestroyWindow(LOGINInput[1]);
		break;
	case LOBBY:
		DestroyWindow(chatInputBoxLobby);
		break;
	case ROOM_WAIT:
		DestroyWindow(chatInputBoxIngame);
		//ShowWindow(chatInputBoxIngame, false);
		break;
	default:
		break;
	}
	scene = _scene;
	SceneInitiator();
}

//���� ���� �������� �ٲ�� �����ؾ��� �κ�
void ServerManager::InitConnection()
{
	//�������� ������ �ޱ�
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
void ServerManager::InputChatting()
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

void ServerManager::ClientExit(POINT pt)
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
void ServerManager::EnterTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;
	bool flag = false;

	//i��°�� �ִ� ���ȣ
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
void ServerManager::ExitTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;
	int i = 0;

	//i��°�� �ִ� ���ȣ
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
void ServerManager::GameStart(POINT pt)
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
			MessageBox(hwnd, "0�� ������ ���� ����.", "?", MB_OK);
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
void ServerManager::Login()
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

void ServerManager::SendPos(int x, int y)
{
	PACKET_SEND_INGAME_DATA packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.playerNum = playerIndex;
	packet.data.wX = x;
	packet.data.wY = y;
	packet.data.roomIndex = roomIndex;
	packet.data.turn = Mystone;

	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}

void ServerManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		//����ü ����
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
//������ ���� ���� ������
bool ServerManager::ProcessPacket(char* szBuf, USER_INFO_STRING& userinfo, int len, WPARAM wParam)
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
	//�� ���δ� ������ ������ �ҵ�
	switch (header.wIndex)
	{
	case PACKET_INDEX_FIRST_CONNECT://ù ����(Ŭ���̾�Ʈ ����)
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, szBuf, header.wLen);

		playerIndex = packet.iIndex;
		char temp[128];
		itoa(playerIndex, temp, 128);
	}
	break;
	case PACKET_INDEX_LOGIN_RET://�α��� �õ�
	{
		PACKET_TRY_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);

		//�α��� ����
		if (packet.isLoginSuccess)
		{
			strcpy(playerID, packet.ID);
			playerIndex = packet.playerNum;
			userIndexInRoom = 0;
			SceneChange(LOBBY);
			GetPlayersInRoom(0);
		}
		else //�α��� ����
		{
			SetWindowText(LOGINInput[0], "�α��� ����");
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
		curTurn = packet.data.turn;
		//gameovercheck
	}
	break;
	case PACKET_INDEX_SEND_CHATTING_LOBBY:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		chatList.push_back(packet.data.chat);
	}
	break;
	case PACKET_INDEX_SEND_CHATTING_INGAME:
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

		//�α��� ��Ŷ�� ���� ������ ������ �߼�
		if (!strcmp(packet.userID, playerID))
			GetRooms();
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
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		if (!packet.isSuccess)
			return true;

		roomIndex = packet.roomIndex;
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
		if (!packet.isSuccess)
			return true;

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
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;
		memcpy(&packet, szBuf, header.wLen);
		isGameStart = true;
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
	break;
	}

	return true;
}
void ServerManager::DrawChatWindow(HDC hdc)
{
	int i = 0;

	switch (scene)
	{
	case LOBBY:
		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			normalFont.Draw((*it), 15, 50, 680 - 30 * i, "Resources/oldgameFont.ttf", RGB(255, 255, 255));
		}
		break;
	case ROOM_WAIT:
		Rectangle(hdc, 750, 400, 950, 700);

		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			normalFont.Draw((*it), 15, 760, 680 - 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	}
}

void ServerManager::DrawCurUsers(HDC hdc)
{
	int i = 0;
	switch (scene)
	{
	case LOGIN:
		break;
	case LOBBY:
		buttonFont.Draw("�κ� �÷��̾� ���", 20, 710, 130, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			buttonFont.Draw((*it), 15, 710, 160 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	case ROOM_WAIT:
		buttonFont.Draw("�ΰ��� �÷��̾� ���", 20, 730, 200, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		if (isGameStart)
		{
			buttonFont.Draw("���� ���� ��", 20, 730, 150, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		else
		{
			buttonFont.Draw("��� ��", 20, 730, 150, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}


		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			normalFont.Draw((*it), 15, 730, 230 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	default:
		break;
	}

}
void ServerManager::DrawRooms(HDC hdc)
{
	char buf[128];

	UIbutton.DrawResizedObject(hdc, 30, 30, 300, 150);
	UIbutton.DrawResizedObject(hdc, 340, 30, 300, 150);
	UIbutton.DrawResizedObject(hdc, 30, 200, 300, 150);
	UIbutton.DrawResizedObject(hdc, 340, 200, 300, 150);
	UIbutton.DrawResizedObject(hdc, 30, 370, 300, 150);
	UIbutton.DrawResizedObject(hdc, 340, 370, 300, 150);

	RECT rect[6];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			rect[i * 2 + j].top = 30 + 170 * i;
			rect[i * 2 + j].left = 30 + 310 * j;
			rect[i * 2 + j].right = 330 + 310 * j;
			rect[i * 2 + j].bottom = 180 + 170 * i;
			roomButtons.push_back(rect[i * 2 + j]);

			sprintf(buf, "%d �� �� �����%d / %d", i * 2 + j, mapRoomPlayers[i * 2 + j], 4);
			normalFont.Draw(buf, 15, rect[i * 2 + j].left + 20, rect[i * 2 + j].top + 20, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
	}

}

void ServerManager::GetPlayersInRoom(int roomNum)
{
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GET_PLAYERS;
	packet.roomIndex = roomNum;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.userID, playerID);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void ServerManager::GetRooms()
{
	PACKET_ROOMLIST packet;

	packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
	packet.header.wLen = sizeof(packet);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
ServerManager::ServerManager() {}

ServerManager::~ServerManager()
{
	closesocket(g_sock);
	WSACleanup();
}