#include "PacketManager.h"
#include "GameManager.h"

using namespace std;
#define BUFSIZE 1024
#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

PacketManager* PacketManager::mthis = nullptr;

void PacketManager::InitConnection(HWND hwnd)
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

	GameManager::GetInstance()->PlayerInit(packet.roomInfo.playerNum, packet.roomInfo.IDs);
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

void PacketManager::SendPos(DRAWPT pt)
{
	PACKET_SEND_INGAME_DATA packet;
	packet.header.wIndex = PACKET_INDEX_SEND_POS;
	packet.header.wLen = sizeof(packet);
	packet.data.DrawPt = pt;
	packet.data.playerNum = userIndexInRoom;
	packet.data.roomIndex = roomIndex;
	packet.data.penInfo = GameManager::GetInstance()->curPenInfo;
	send(g_sock, (const char*)& packet, packet.header.wLen, 0);
}

void PacketManager::SendDump(int r, int g, int b)
{
	PACKET_DUMP_ALL packet;
	packet.header.wIndex = PACKET_INDEX_DUMP;
	packet.header.wLen = sizeof(packet);
	packet.penInfo.r = r;
	packet.penInfo.g = g;
	packet.penInfo.b = b;
	packet.data.roomIndex = roomIndex;
	send(g_sock, (const char*)& packet, packet.header.wLen, 0);
}

void PacketManager::SendChattingData(char *str)
{
	PACKET_SEND_INGAME_DATA packet;
	
	packet.header.wIndex = PACKET_INDEX_SEND_CHATTING;
	packet.header.wLen = sizeof(packet);
	packet.data.userIndexInRoom = userIndexInRoom;
	packet.data.roomIndex = roomIndex;

	//strcpy(packet.data.ID, playerID);
	memcpy(packet.data.ID, playerID, sizeof(packet.data.ID));
	//strcpy(packet.data.chat, str);
	memcpy(packet.data.chat, str, sizeof(packet.data.chat));
	
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
//������ ���� ���� ������
bool PacketManager::ProcessPacket(char* szBuf, int len, WPARAM wParam)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;
	cout << "��Ŷ����." << len << endl;

	// ª��
	if (myLen < sizeof(PACKET_HEADER))
	{
		cout << "��Ŷ����. ������� ª��" << endl;
		return false;
	}

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));
	
	if (myLen < header.wLen)
	{
		cout << "��Ŷ����. header.wLen���� ª��" << endl;
		return false;
	}

	// ��
	if (myLen > header.wLen)
	{
		cout << "��Ŷ����. header.wLen���� ��." << endl;
		cout << "myLen: " << myLen << " / header.wLen: " << header.wLen
			<< "header.wIndex: " << header.wIndex << endl;
	}

	switch (header.wIndex)
	{
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
		
		GameManager::GetInstance()->whiteBoardDraw(packet.data.DrawPt, packet.data.penInfo);
	}
	break;
	case PACKET_INDEX_DUMP:
	{
		PACKET_DUMP_ALL packet;
		memcpy(&packet, szBuf, header.wLen);

		GameManager::GetInstance()->DumpAll(packet.penInfo.r, packet.penInfo.g, packet.penInfo.b);
	}
	break;
	case PACKET_INDEX_GET_ROOMS:
	{
		PACKET_ROOMLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		GameManager::GetInstance()->RoomListInit(packet.NumOfRoom, packet.playerNum);
		GameManager::GetInstance()->RoomButtonUpdate();
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;
		memcpy(&packet, szBuf, header.wLen);
		
		if (userIndexInRoom == 0)
			strcpy(answer, packet.answer);
		
		isGameStart = true;
		curTurn = 0;
		score = 0;
		GameManager::GetInstance()->SceneChange(PLAYING);
	}
	break;
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_TRY_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);
		
		//�α��� ����
		if (packet.isSuccess)
		{
			strcpy(playerID, packet.ID);
			userIndexInRoom = packet.playerIndexInroom;
			GetPlayersInRoom(0); //0���� (�κ�) �÷��̾� ���� ��������
			GameManager::GetInstance()->SceneChange(LOBBY);
		}
		else //�α��� ����
		{
			//MessageBox(hwnd, "?","�α��� ����", MB_OK);
			//GameManager�� ó��
		}
	}
	break;
	case PACKET_INDEX_SEND_CHATTING:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);
		GameManager::GetInstance()->ReceiveChatStr(packet.data.chat);
		
		if (packet.answerIsCorrect)
		{
			//�����ڰ� �ƴ� ������
			if (strcmp(packet.data.ID, playerID))
			{
				//���� ȿ��
				score++;
			}
			//�ð�, ��ȭ�� ����
			GameManager::GetInstance()->TimeOver(ResourceManager::GetInstance()->backBuffer->GetmemDC());
			curTurn = packet.CurTurn;
			//���亯��
			//������
			if (curTurn == userIndexInRoom)
			{
				strcpy(answer, packet.answer);
			}
			else
				strcpy(answer, "?");
		}
		if (packet.isGameOver)
		{
			GameManager::GetInstance()->GameOver();
		}
	}
	break;
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		GameManager::GetInstance()->PlayerInit(packet.roomInfo.playerNum, packet.roomInfo.IDs);


		//�α��� ��Ŷ�� ���� ������ ������ �߼�
		if (!strcmp(packet.playerID, playerID))
			GetRooms();
	}
	break;
	case PACKET_INDEX_GAMEEXIT:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		GameManager::GetInstance()->PlayerInit(packet.roomInfo.playerNum, packet.roomInfo.IDs);
		//strcpy(packet.playerID, playerID);
		
		closesocket(wParam);
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		
		roomIndex = packet.roomInfo.roomIndex;
		GameManager::GetInstance()->PlayerInit(packet.roomInfo.playerNum, packet.roomInfo.IDs);

		if (!strcmp(packet.playerID, playerID))
		{
			userIndexInRoom = packet.userIndexInRoom;
			GameManager::GetInstance()->SceneChange(ROOM_WAIT);
		}
		else
			GetRooms();
	}
	break;
	case PACKET_INDEX_EXIT_THE_ROOM:
	{
		PACKET_TRY_EXIT_THE_ROOM packet;

		memcpy(&packet, szBuf, header.wLen);

		roomIndex = packet.roomInfo.roomIndex;
		GameManager::GetInstance()->PlayerInit(packet.roomInfo.playerNum, packet.roomInfo.IDs);

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
	case PACKET_INDEX_TIMER:
	{
		PACKET_TIMER packet;
		memcpy(&packet, szBuf, header.wLen);
		curTurn = packet.CurTurn;
		GameManager::GetInstance()->TimeOver(ResourceManager::GetInstance()->backBuffer->GetmemDC());
		
		//�ڽ��� ���� ��� ��� ���
		if(isMyTurn())
			strcpy(answer, packet.answer);
		else
			strcpy(answer, "?");

		if (packet.isGameOver)
		{
			isGameStart = false;
			GameManager::GetInstance()->GameOver();
		}
	}
	break;
	}
	myLen -= len;
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

bool PacketManager::isMyTurn()
{
	if (curTurn == userIndexInRoom)
		return true;
	return false;
}

void PacketManager::Release()
{
	closesocket(g_sock);
	WSACleanup();
	delete this;
}
PacketManager::PacketManager() {}
PacketManager::~PacketManager() {}

/*
void PacketManager::ClientExit(POINT pt)//���� ���� ��ư (����)
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