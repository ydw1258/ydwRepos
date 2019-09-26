#include "GameManager.h"
#include "ResourceManager.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include <string>
#include <Windows.h>
#include<iostream>

using namespace std;
#define BUFSIZE 1024

GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[7] = { "Resources\\board.bmp", "Resources\\blackstone.bmp", "Resources\\whitestone.bmp",
		"Resources\\UIButton.bmp", "Resources\\memo.bmp", "Resources\\lobbybackground.bmp", "Resources\\blueBoard.bmp"
	};
	ResourceManager::GetInstance()->Init(hdc, hInstance, filename, 7);
	
	board.Init(IMAGENUM_BOARD, 1, 722, 720);
	blackStone.Init(IMAGENUM_BLACKSTONE, 1, stoneSizeXY, stoneSizeXY);
	whiteStone.Init(IMAGENUM_WHITESTONE, 1, stoneSizeXY, stoneSizeXY);
	UIbutton.Init(IMAGENUM_UIBUTTON, 1, 346, 173);
	memoImage.Init(IMAGENUM_MEMO, 1, 400, 400);
	lobbybackground.Init(IMAGENUM_LOBBYBACKGROUND, 1, 960, 640);
	blueBoard.Init(IMAGENUM_CHATBOARD_LOBBY, 1, 300, 111);
	InitConnection();
	SceneInitiator();
}
void GameManager::SceneInitiator()
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
		//chatInputBox = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case INGAME:
		chatInputBoxIngame = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		break;
	}
}
void GameManager::Draw(HDC hdc)
{
	switch (scene)
	{
	case LOGIN:
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		break;
	case LOBBY:
	{
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		memoImage.DrawResizedObject(hdc, 650, 10, 300, 400);
		blueBoard.DrawResizedObject(hdc, 40, 520, 700, 200);

		DrawChatWindow(hdc);
		DrawRooms(hdc);
		DrawCurUsers(hdc);
	}
	break;
	case INGAME:
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		board.DrawObject(hdc, 0, 0);
		font.Draw(playerID, 30, 800, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));

		char buf[128];
		sprintf(buf, "%d 번방", curRoomNum);
		font.Draw(buf, 30, 800, 130, "Resources/oldgameFont.ttf", RGB(255, 0, 0));

		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				switch (BoardInfo[HEIGHT * i + j])
				{
				case 0://비어있음.
					break;
				case 1://흑
					blackStone.DrawObject(hdc, j * stoneSizeXY + 8, i * stoneSizeXY + 8);
					break;
				case 2://백
					whiteStone.DrawObject(hdc, j * stoneSizeXY + 8, i * stoneSizeXY + 8);
					break;
				}
			}
		}
		DrawCurUsers(hdc);
		DrawChatWindow(hdc);
		break;
	}
}
void GameManager::DrawRect(HDC hdc)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			RECT rect;
			rect.top = i * stoneSizeXY;
			rect.bottom = (i + 1) * stoneSizeXY;
			rect.left = (j * stoneSizeXY);
			rect.right = (j + 1) * stoneSizeXY;
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
	}
}
void GameManager::MouseButtonCheck(POINT pt)
{
	if (curTurn == Mystone || GetForegroundWindow() != hwnd)
		return;
	
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			RECT rect;
			rect.top = i * stoneSizeXY;
			rect.bottom = (i + 1) * stoneSizeXY;
			rect.left = (j * stoneSizeXY);
			rect.right = (j + 1) * stoneSizeXY;

			if (Physics::GetInstance()->RECTbyPointCollisionCheck(rect, pt))
			{
				if (BoardInfo[i * HEIGHT + j] == 0)
				{
					if (Mystone)
						BoardInfo[i * HEIGHT + j] = 0; //흑
					else
						BoardInfo[i * HEIGHT + j] = 1; //백
					SendPos(j, i);
					GameOverCheck();
				}
				return;
			}
		}
	}
}
void GameManager::GameOverCheck()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (BoardInfo[i * HEIGHT + j] != 0)
			{
				//오른쪽, 아래, 오른쪽대각선, 왼쪽 대각선 확인
				if (BoardInfo[i * HEIGHT + j] == BoardInfo[i * HEIGHT + j + 1]		  &&	BoardInfo[i * HEIGHT + j + 2]		&&	BoardInfo[i * HEIGHT + j + 3] && BoardInfo[i * HEIGHT + j + 4] ||
					BoardInfo[i * HEIGHT + j] == BoardInfo[(i + 1) * HEIGHT + j] && BoardInfo[(i + 2) * HEIGHT + j] && BoardInfo[(i + 3) * HEIGHT + j] && BoardInfo[(i + 4) * HEIGHT + j] ||
					BoardInfo[i * HEIGHT + j] == BoardInfo[(i + 1) * HEIGHT + j + 1] && BoardInfo[(i + 2) * HEIGHT + j + 2] && BoardInfo[(i + 3) * HEIGHT + j + 3] && BoardInfo[(i + 4) * HEIGHT + j + 4] ||
					j >= 4 && (BoardInfo[i * HEIGHT + j] == BoardInfo[(i + 1) * HEIGHT + j - 1] && BoardInfo[(i + 2) * HEIGHT + j - 2] && BoardInfo[(i + 3) * HEIGHT + j - 3] && BoardInfo[(i + 4) * HEIGHT + j - 4]))
				{
					MessageBox(hwnd, "흑이 이겼습니다.", "?", MB_OK);
				}
			}
		}
	}
}
void GameManager::DrawChatWindow(HDC hdc)
{
	int i = 0;
	switch (scene)
	{
	case LOBBY:
		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			font.Draw((*it), 15, 50, 680 - 30 * i, "Resources/oldgameFont.ttf", RGB(255, 255, 255));
		}
		break;
	case INGAME:
		Rectangle(hdc, 750, 400, 950, 700);

		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			font.Draw((*it), 15, 760, 680 - 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	default:
		break;
	}
}

void GameManager::DrawCurUsers(HDC hdc)
{
	int i = 0;
	switch (scene)
	{
	case LOGIN:
		break;
	case LOBBY:
		font.Draw("로비 플레이어 목록", 20, 710, 130, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			font.Draw((*it), 15, 710, 160 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	case INGAME:
		font.Draw("인게임 플레이어 목록", 20, 730, 200, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			font.Draw((*it), 15, 730, 230 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	default:
		break;
	}
	
}
void GameManager::DrawRooms(HDC hdc)
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

			sprintf(buf, "%d 번 방 방제.", i * 2 + j + 1);
			font.Draw(buf, 15, rect[i * 2 + j].left + 20, rect[i * 2 + j].top + 20, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
	}
	//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
}
void GameManager::EnterTheRoom(POINT pt)
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
	if (flag == false)
		return;
	char buf[128];

	PACKET_TRY_ENTER_THE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_ENTER_THE_ROOM;
	packet.header.wLen = sizeof(packet);
	strcpy(packet.playerID, playerID);
	packet.roomNum = i + 1; 
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
}
void GameManager::Login()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char buf[128];

	PACKET_TRY_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
	packet.header.wLen = sizeof(packet);
	
	GetWindowText(LOGINInput[0], buf, 128);
	strcpy(packet.ID, buf);
	GetWindowText(LOGINInput[1], buf, 128);
	strcpy(packet.password, buf);
	
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
}

void GameManager::SendPos(int x, int y)
{
	PACKET_SEND_INGAME_DATA packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.playerNum = playerIndex;
	packet.data.wX = x;
	packet.data.wY = y;
	packet.data.turn = Mystone;
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
}

void GameManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
			}
		}

		ProcessPacket(szBuf, retval);
	break;
	case FD_CLOSE:
		closesocket(wParam);
		break;
	}
}
//서버로 부터 받은 데이터
void GameManager::ProcessPacket(char * szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

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
		if (packet.isLoginSuccess)
		{
			strcpy(playerID, packet.ID);
			playerIndex = packet.playerNum;
			SceneChange(LOBBY);
			
			GetPlayersInRoom(0);
			GetRooms();
		}
		else //로그인 실패
		{
			SetWindowText(LOGINInput[0], "로그인 실패");
		}

		if (playerIndex == 0)//받은 데이터 흑
		{
			Mystone = 0;
			curTurn = Mystone;
		}
		else if (playerIndex == 1)
		{
			Mystone = 1;
			curTurn = false;
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
		BoardInfo[packet.data.wY * HEIGHT + packet.data.wX] = packet.data.turn + 1;
		curTurn = packet.data.turn;
	}
	break;
	case PACKET_INDEX_SEND_CHATTING_INGAME:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		chatList.push_back(packet.data.chat);
	}
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		listPlayerID.clear();

		for (int i = 0; i < packet.playerNum; i++)
		{
			listPlayerID.push_back(packet.playerIDs[i]);
		}
	}
	break;
	case PACKET_INDEX_GET_ROOMS:
	{
		PACKET_ROOMLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		for (int i = 0; i < packet.NumOfRoom; i++)
		{
			mapRoomPlayers.insert(make_pair(packet.roomNum[i], packet.playerNum[i]));
		}
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		if (!packet.isSuccess)
			return;
		curRoomNum = packet.roomNum;

		listPlayerID.clear();
		for (int i = 0; i < packet.userNumInRoom; i++)
		{
			listPlayerID.push_back(packet.ID[i]);
		}
		SceneChange(INGAME);
		SceneInitiator();
	}
	break;
	}
}
void GameManager::SceneChange(Scene _scene)
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
	case INGAME:
		DestroyWindow(chatInputBoxIngame);
		//ShowWindow(chatInputBoxIngame, false);
		break;
	default:
		break;
	}
	scene = _scene;
	SceneInitiator();
}
void GameManager::GetPlayersInRoom(int roomNum)
{
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GET_PLAYERS;
	packet.roomNum = roomNum;
	packet.header.wLen = sizeof(packet);
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
}
void GameManager::GetRooms()
{
	PACKET_ROOMLIST packet;

	packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
	packet.header.wLen = sizeof(packet);
	send(g_sock, (const char*)& packet, sizeof(packet), 0);
}
void GameManager::InitConnection()
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
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

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
void GameManager::InputChatting()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char str[128];

	switch (scene)
	{
	case LOBBY:
	{
		
	}
	break;
	case INGAME:
	{
		PACKET_SEND_INGAME_DATA packet;

		GetWindowText(chatInputBoxIngame, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		packet.header.wIndex = PACKET_INDEX_SEND_CHATTING_INGAME;
		packet.header.wLen = sizeof(packet);
		packet.data.playerNum = playerIndex;

		strcpy(packet.data.ID, playerID);
		strcpy(packet.data.chat, str);

		send(g_sock, (const char*)&packet, sizeof(packet), 0);
		SetWindowText(chatInputBoxIngame, "");
	}
	break;
	}
}

GameManager::GameManager(){}

GameManager::~GameManager()
{
	closesocket(g_sock);
	WSACleanup();
}
