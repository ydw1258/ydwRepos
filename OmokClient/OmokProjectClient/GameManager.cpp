#include "GameManager.h"
#include "ResourceManager.h"
#include "IMAGENUM.h"
#include "Physics.h"
#include <string>
#include <Windows.h>
#include "..\..\Common\PACKET_HEADER.h"

using namespace std;
#define BUFSIZE 512

SOCKET g_sock;

GameManager* GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[3] = {"Resources\\board.bmp", "Resources\\blackstone.bmp", "Resources\\whitestone.bmp"};
	ResourceManager::GetInstance()->Init(hdc, hInstance, filename, 3);
	board.Init(IMAGENUM_BOARD, 1, 722, 720);
	blackStone.Init(IMAGENUM_BLACKSTONE, 1, stoneSizeXY, stoneSizeXY);
	whiteStone.Init(IMAGENUM_WHITESTONE, 1, stoneSizeXY, stoneSizeXY);

	//서버한테 데이터 받기
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET)
	{
		//cout << "err on socket" << endl;
		return;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int retval = connect(g_sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		//cout << "err on connect" << endl;
		return;
	}

	retval = WSAAsyncSelect(g_sock, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		return;
	}

	retval = WSAAsyncSelect(g_sock, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		return;
	}
	closesocket(g_sock);
	WSACleanup();

	if (1)//받은 데이터 흑
	{
		isMyTurn = true;
		Mystone = 0;
	}
	
}

void GameManager::Draw(HDC hdc)
{
	board.DrawObject(hdc, 0, 0);
	int a = 0;

	for (int i = 0; i < HEIGHT; i++)
	{
		for(int j = 0; j < WIDTH; j++)
		{
			switch (BoardInfo[HEIGHT * i + j])
			{
			case 0://비어있음.
				a++;
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
}

void GameManager::MouseButtonCheck(POINT pt)
{
	if (!isMyTurn)
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
void GameManager::SendPos(int x, int y)
{
	PACKET_SEND_POS packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.iIndex = g_iIndex;
	packet.data.wX = x;
	packet.data.wY = y;
	send(g_sock, (const char*)&packet, sizeof(packet), 0);
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

		g_iIndex = packet.iIndex;
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
		PACKET_SEND_POS packet;
		memcpy(&packet, szBuf, header.wLen);
	}
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
GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}
