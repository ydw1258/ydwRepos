#include "ServerManager.h"

ServerManager* ServerManager::mthis = nullptr;

ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void ServerManager::Init(HWND hwnd)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "Init failed" << endl;
		return;
	}
		

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		return;
	}

	retval = WSAAsyncSelect(listen_sock, hwnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on WSAAsyncSelect" << endl;
		return;
	}

	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		cout << "err on bind" << endl;
		return;
	}

	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on listen" << endl;
		return;
	}
}

void ServerManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;

	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		if (err_code == 10053) //Ŭ�� ������ ��
		{
			g_iIndex--;
			closesocket(wParam);
		}

		err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT: //Ŭ�� ����
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			int err_code = WSAGetLastError();
			if (err_code != WSAEWOULDBLOCK)
			{
				err_display("err on accept");
			}
			return;
		}
		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s , ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on WSAAsyncSelect!!" << endl;
		}

		USER_INFO* pInfo = new USER_INFO();
		pInfo->index = g_iIndex++;
		pInfo->len = 0;
		pInfo->x = rand() % 600;
		pInfo->y = rand() % 400;
		g_mapUser.insert(make_pair(client_sock, pInfo));

		PACKET_LOGIN_RET packet;
		packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
		packet.header.wLen = sizeof(packet);
		packet.iIndex = pInfo->index;

		send(client_sock, (const char*)&packet, packet.header.wLen, 0);

		Sleep(500);

		PACKET_USER_DATA user_packet;
		user_packet.header.wIndex = PACKET_INDEX_USER_DATA;
		user_packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(WORD) + sizeof(USER_DATA) * g_mapUser.size();
		user_packet.wCount = g_mapUser.size();

		int i = 0;
		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++, i++)
		{
			user_packet.data[i].playerNum = iter->second->index;
			user_packet.data[i].wX = iter->second->x;
			user_packet.data[i].wY = iter->second->y;
			user_packet.data[i].turn = 0;
		}

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++, i++)
		{
			send(iter->first, (const char*)&user_packet, user_packet.header.wLen, 0);
		}
	}
	break;
	case FD_READ:// Ŭ�� ��Ŷ �б�
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				cout << "err on recv!!" << endl;
			}
		}

		USER_INFO* pUser = g_mapUser[wParam];

		while (true)
		{
			if (!ProcessPacket(wParam, pUser, szBuf, retval))
			{
				Sleep(100);
				//SendMessage(hWnd, uMsg, wParam, lParam);
				break;
			}
			else
			{
				if (pUser->len < sizeof(PACKET_HEADER))
					break;
			}
		}

	}
	break;
	case FD_CLOSE:
		g_iIndex--;
		closesocket(wParam);
		break;
	}
}
//Ŭ�� ���� ��Ŷ
bool ServerManager::ProcessPacket(SOCKET sock, USER_INFO* pUser, char* szBuf, int& len)
{
	if (len > 0)
	{
		memcpy(&pUser->szBuf[pUser->len], szBuf, len);
		pUser->len += len;
		len = 0;
	}

	if (pUser->len < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, pUser->szBuf, sizeof(header));

	if (pUser->len < header.wLen)
		return false;

	
	switch (header.wIndex)
	{
	case PACKET_INDEX_SEND_POS:
		PACKET_SEND_POS packet;
		memcpy(&packet, szBuf, header.wLen);

		g_mapUser[sock]->x = packet.data.wX;
		g_mapUser[sock]->y = packet.data.wY;

		turn = !packet.data.turn;
		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			//if (iter->first == sock)
				//continue;

			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	break;
	case PACKET_INDEX_SEND_CHATTING_INGAME:
		PACKET_SEND_CHAT_MESSAGE chatPacket;
		memcpy(&chatPacket, szBuf, header.wLen);
		// chatPacket.data.playerNum + " : " + chatPacket.chatMessage


		//strcpy(chatPacket.chatMessage, chatMessage);
		
		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			//if (iter->first == sock)
				//continue;

			send(iter->first, (const char*)&chatPacket, header.wLen, 0);
		}
		break;
	}

	memcpy(&pUser->szBuf, &pUser->szBuf[header.wLen], pUser->len - header.wLen);
	pUser->len -= header.wLen;

	return true;
}

void ServerManager::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[����]%s", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void ServerManager::err_display(char* szMsg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s\n", szMsg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}