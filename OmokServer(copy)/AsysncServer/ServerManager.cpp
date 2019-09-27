#include "ServerManager.h"
ServerManager* ServerManager::mthis = nullptr;

ServerManager::ServerManager(){}
ServerManager::~ServerManager(){}

void ServerManager::Init(HWND _hWnd)
{
	hWnd = _hWnd;
	
	//�޸��忡 ����� �������� �о����
	ifstream openFile("accounts.txt");
	if (openFile.is_open())
	{
		string line;
		while (getline(openFile, line))
		{
			int idx = line.find("/");

			mapAccounts.insert(make_pair(line.substr(0, idx), line.substr(idx + 1, line.length())));
		}
	}

	//���� ���� ����
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		exit(1);
	}

	int retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on WSAAsyncSelect" << endl;
		exit(1);
	}

	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	ShowWindow(hWnd, SW_HIDE);

	if (retval == SOCKET_ERROR)
	{
		cout << "err on bind" << endl;
		exit(1);
	}

	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on listen" << endl;
		exit(1);
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
		err_display(WSAGETSELECTERROR(lParam));
		if(WSAGETSELECTERROR(lParam) == 10053)
		{ 
			//�������� ����
			map<int, list<USER_INFO>> g_RoomInfo;

			g_iIndex--;
			for (auto it = g_RoomInfo[g_mapUser[wParam]->roomIndex].begin(); it != g_RoomInfo[g_mapUser[wParam]->roomIndex].end(); it++)
			{
				if (it->userID == g_mapUser[wParam]->userID)
					g_RoomInfo[g_mapUser[wParam]->roomIndex].erase(it);
			}

			g_mapUser.erase(wParam);
			closesocket(wParam);
		}
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	//���� ��û�� Ŭ�� �ޱ�
	case FD_ACCEPT:
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
		//���� �õ��� Ŭ�󿡰� index�Ҵ�
		USER_INFO* pInfo = new USER_INFO();
		pInfo->index = g_iIndex++;
		pInfo->len = 0;
		g_mapUser.insert(make_pair(client_sock, pInfo));

		PACKET_LOGIN_RET packet;
		packet.header.wIndex = PACKET_INDEX_FIRST_CONNECT;
		packet.header.wLen = sizeof(packet);
		packet.iIndex = pInfo->index;
		send(client_sock, (const char*)&packet, packet.header.wLen, 0);
	}
	break;
	case FD_READ:
	{
		char szBuf[BUFSIZE];

		//������ ��Ŷ�� ����Ʈ ��
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
		closesocket(wParam);
		break;
	}
}
//Ŭ�� ���� ��Ŷ �б�
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
	case PACKET_INDEX_LOGIN_RET:
	{
		PACKET_TRY_LOGIN loginPacket;
		memcpy(&loginPacket, szBuf, header.wLen);
		PACKET_USER_DATA loginRetPacket;
		
		cout << "�Էµ� ID : " << loginPacket.ID << ", " <<
			"�Էµ� ��й�ȣ : "<< loginPacket.password << ", ������й�ȣ : " << mapAccounts[loginPacket.ID] << endl;
		if (mapAccounts.find(loginPacket.ID) == mapAccounts.end()) {
			//�α��� ����
			cout << "�α��� ����" << endl;
		}
		else {
			//��� Ʋ��
			if (strcmp(mapAccounts[loginPacket.ID].c_str(), loginPacket.password))
			{
				//loginRetPacket.isLoginSuccess = false;
				//loginRetPacket.header.wIndex = PACKET_INDEX_LOGIN_RET;
				cout << "��й�ȣ Ʋ��" << endl;
			}
			// �α��� ����
			else
			{
				USER_INFO *user = new USER_INFO();
				//g_mapUser[sock]->userID = ;
				loginPacket.isLoginSuccess = true;
				strcpy(user->userID, loginPacket.ID);
				user->roomIndex = 0;
				user->userCurScene = LOBBY;
				g_mapUser[sock] = user;

				//�κ� ���� �߰�
				// map<int, list<USER_INFO*>> g_RoomInfo; //0�� �κ�
				g_RoomInfo[0].push_back(user);
				loginPacket.roomIndex = roomIndexes[0]++;
				cout << "�α��� ����" << endl;
			}
		}
		loginRetPacket.header.wLen = sizeof(loginRetPacket);
		
		send(sock, (const char*)& loginPacket, header.wLen, 0);
	}
	break;
	case PACKET_INDEX_SEND_POS:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		//g_mapUser[sock]->
		turn = !packet.data.turn;

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			//if (iter->first == sock)
				//continue;

			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_SEND_CHATTING_LOBBY:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		char buf[128];
		sprintf(buf, "%s : %s", packet.data.ID, packet.data.chat);
		strcpy(packet.data.chat, buf);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomIndex != g_mapUser[sock]->roomIndex)
				continue;

			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_SEND_CHATTING_INGAME:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		char buf[128];
		sprintf(buf, "%s : %s", packet.data.ID, packet.data.chat);
		strcpy(packet.data.chat, buf);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomIndex != g_mapUser[sock]->roomIndex)
				continue;

			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_GET_PLAYERS:
	{
		PACKET_USERSLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		int i = 0;
		for (auto it = g_RoomInfo[packet.roomNum].begin(); it != g_RoomInfo[packet.roomNum].end(); it++, i++)
		{
			strcpy(packet.playerIDs[i], (*it)->userID);
		}
		packet.playerNum = i;

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++, i++)
		{
			if(it->second->roomIndex == packet.roomNum)
				send(it->first, (const char*)& packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_GET_ROOMS:
	{
		PACKET_ROOMLIST packet;
		memcpy(&packet, szBuf, header.wLen);
		
		//���� �� 6��
		for (int i = 0; i < 6; i++)
		{
			packet.roomNum[i] = i + 1;
			packet.playerNum[i] = g_RoomInfo[i].size();
		}
		packet.NumOfRoom = 6; //�׽�Ʈ��
		send(sock, (const char*)&packet, header.wLen, 0);
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM:
	{
		PACKET_TRY_ENTER_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);

		//�������� ����(room)
		g_mapUser[sock]->roomIndex = packet.roomIndex;
		//�濡�߰�
		g_RoomInfo[packet.roomIndex].push_back(g_mapUser[sock]);
		int i = 0;

		for (auto it = g_RoomInfo[packet.roomIndex].begin(); it != g_RoomInfo[packet.roomIndex].end(); it++, i++)
		{
			strcpy(packet.ID[i], (*it)->userID);
		}
		packet.userIndexInRoom = i - 1;
		
		//�κ񿡼� ����
		
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			
			it = g_RoomInfo[0].erase();
		}
		
		packet.isSuccess = true;
		//userIndex�� �� ���Ը�
		send(sock, (const char*)&packet, header.wLen, 0);

		
		//���������� ���� �����;���
		/*
		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomIndex != packet.roomIndex)
				continue;
			send(iter->first, (const char*)&packet, header.wLen, 0);
		}
		*/
	}
	break;
	case PACKET_INDEX_EXIT_THE_ROOM:
	{
		PACKET_TRY_EXIT_THE_ROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		
		//retPacket.
		//�������� ����(room)
		g_mapUser[sock]->roomIndex = 0;
		
		//�濡�� ����
		for (auto it = g_RoomInfo[packet.roomIndex].begin(); it != g_RoomInfo[packet.roomIndex].end(); )
		{
			if(packet.playerID == (*it)->userID)
			{
				it = g_RoomInfo[packet.roomIndex].erase(it);
			}
			else 
				it++;
		}
		g_RoomInfo[0].push_back(g_mapUser[sock]);
		
		send(sock, (const char*)&packet, header.wLen, 0);
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;
		memcpy(&packet, szBuf, header.wLen);
		//retPacket.
		//�������� ����(room)
		g_mapUser[sock]->roomIndex = 0;
		
		if (packet.userIndexInRoom == 0)
		{
			packet.MyStone = 0;
		}
		else if (packet.userIndexInRoom == 1)
		{
			packet.MyStone = 1;
		}
			
		//�濡�ִ� �÷��̾� �鿡�� ���ӽ��� ������
		//�÷��̾� 0, 1���Ը� �������� ����
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == g_mapUser[sock]->roomIndex)
			{
				send(it->first, (const char*)&packet, header.wLen, 0);
			}
		}
		//send(sock, (const char*)&packet, header.wLen, 0);
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