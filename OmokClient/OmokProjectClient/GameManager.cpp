#include "GameManager.h"
#include "ResourceManager.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include <string>
#include <Windows.h>
#include<iostream>

using namespace std;
#define BUFSIZE 512

GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[3] = {"Resources\\board.bmp", "Resources\\blackstone.bmp", "Resources\\whitestone.bmp"};
	ResourceManager::GetInstance()->Init(hdc, hInstance, filename, 3);
	
	board.Init(IMAGENUM_BOARD, 1, 722, 720);
	blackStone.Init(IMAGENUM_BLACKSTONE, 1, stoneSizeXY, stoneSizeXY);
	whiteStone.Init(IMAGENUM_WHITESTONE, 1, stoneSizeXY, stoneSizeXY);

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
	case INGAME:
		chatInputBox = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case LOBBY:
		break;
	default:
		break;
	}
}
void GameManager::Draw(HDC hdc)
{
	switch (scene)
	{
	case LOGIN:
		break;
	case INGAME:
		board.DrawObject(hdc, 0, 0);
		font.Draw(playerIndex, 30, 800, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));

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
		DrawChatWindow(hdc);
		break;
	case LOBBY:
		break;
	default:
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
	Rectangle(hdc, 750, 400, 950, 700);
	int i = 0;
	for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
	{
		font.Draw((*it), 15, 760, 680 - 30 *i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
	}
}
void GameManager::DrawLobbyChatWindow(HDC hdc)
{
}
void GameManager::DrawCurUsers(HDC hdc)
{
}
void GameManager::DrawRooms(HDC hdc)
{
}
void GameManager::Login()
{
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
//서버
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
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_LOGIN_RET packet;
		memcpy(&packet, szBuf, header.wLen);

		//로그인 실패
		//구현
		//로그인 성공

		playerIndex = packet.iIndex;

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
		if (packet.isLoginSuccess)
		{
			SceneChange();
		}
		else
		{
			SetWindowText(LOGINInput[0], "로그인 실패");
		}
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
		
	break;
	}
}
void GameManager::SceneChange()
{
	switch (scene)
	{
	case LOGIN:
		scene = LOBBY;
		DestroyWindow(LOGINInput[0]);
		DestroyWindow(LOGINInput[1]);
		break;
	case LOBBY:
		break;
	case INGAME:
		break;
	default:
		break;
	}
	
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

void GameManager::InputChatting(void)
{
	if (GetForegroundWindow() != hwnd)
		return;
	char str[128];
	GetWindowText(chatInputBox, str, 128);

	PACKET_SEND_INGAME_DATA packet;
	packet.header.wIndex = PACKET_INDEX_SEND_CHATTING_INGAME;
	packet.header.wLen = sizeof(packet);
	packet.data.playerNum = playerIndex;
	strcpy(packet.data.chat, str);

	send(g_sock, (const char*)&packet, sizeof(packet), 0);
	
	SetWindowText(chatInputBox, "");
}

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	closesocket(g_sock);
	WSACleanup();
}
