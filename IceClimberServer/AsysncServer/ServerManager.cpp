#include "ServerManager.h"
ServerManager* ServerManager::mthis = nullptr;

//�����庰 ����Ʈ ��Ʈ�� ó��, �����尡 �������� ��Ⱑ �߻��ϸ� CompletionPort �Լ��� ��������
//������ ������ �̸��صΰ� ���� �ִ� �����带 ���� ��Ŷ�� �ް� ó��
//ũ��Ƽ�� ���� => ���� �Ǵ� �κ� (���� ���� �����) LOCK

ServerManager::ServerManager(){}
ServerManager::~ServerManager(){}
DWORD WINAPI WorkerThread(LPVOID arg);

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

	int retval;
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	//����� �Ϸ� ��Ʈ ����
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return;

	//CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//�۾��� ������ ���� ���ǽ� ���� 16�� ����
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		//createThread ���װ� �־ ���� �ʿ�
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL)
			return;
		CloseHandle(hThread);
	}

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_sock == INVALID_SOCKET)
	{
		//err_quit((char *)"socket()");
	}
	
	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
	{
		//err_quit((char *)"listen()");
	}
}

void ServerManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;

	DWORD cbTransferred;//���۷�
	SOCKET client_sock;

	SOCKETINFO *ptr;
	retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

	if (retval == 0 || cbTransferred == 0)
	{
		if (retval == 0)
		{
			DWORD temp1, temp2;

			WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
			err_display((char *)"WSAGetOverlappedResult()");
		}
		closesocket(ptr->sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		delete ptr;
		return;
	}

	if (ptr->recvbytes == 0)
	{
		ptr->recvbytes = cbTransferred;
		ptr->sendbytes = 0;

		ptr->buf[ptr->recvbytes] = '\0';
		printf("[TCP \%s : %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
	}
	else
	{
		ptr->sendbytes += cbTransferred;
	}

	if (ptr->recvbytes > ptr->sendbytes)
	{
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
		ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

		DWORD sendbytes;
		retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display((char *)"WSASend()");
			}
			return;
		}
		else
		{
			ptr->recvbytes = 0;

			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			DWORD flags = 0;

			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					err_display((char *)"WSARecv()");
				}
				return;
			}

		}
	}
	return;


	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		err_display(WSAGETSELECTERROR(lParam));

		if(WSAGETSELECTERROR(lParam) == 10053)
		{ 
			//�������� ����
			PACKET_USERSLIST packet;
			packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
			packet.header.wLen = sizeof(packet);
			packet.roomIndex = g_mapUser[wParam]->roomIndex;
			
			//�ش� �濡�� ����
			for (auto it = g_RoomInfo[g_mapUser[wParam]->roomIndex].begin(); it != g_RoomInfo[g_mapUser[wParam]->roomIndex].end(); it++)
			{
				if (!strcmp((*it)->userID, g_mapUser[wParam]->userID))
				{
					g_RoomInfo[g_mapUser[wParam]->roomIndex].erase(it);
					break;
				}
			}

			int i = 0;
			for (auto it = g_RoomInfo[g_mapUser[wParam]->roomIndex].begin(); it != g_RoomInfo[g_mapUser[wParam]->roomIndex].end(); it++, i++)
			{
				strcpy(packet.playerIDs[i], (*it)->userID);
			}
			packet.playerNum = i;

			for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
			{
				if (it->second->roomIndex == g_mapUser[wParam]->roomIndex)
				{
					send(it->first, (const char*)& packet, packet.header.wLen, 0);
				}
			}
			
			//�� ���� ���� ����
			if (g_mapUser[wParam]->roomIndex != 0)
			{
				PACKET_ROOMLIST packet;
				packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
				packet.header.wLen = sizeof(packet);
				//���� �� 6��
				for (int i = 0; i < 6; i++)
				{
					packet.roomIndex[i] = i;
					packet.playerNum[i] = g_RoomInfo[i + 1].size();
				}
				packet.NumOfRoom = 6; //�׽�Ʈ��

				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (it->second->roomIndex == 0)
						send(it->first, (const char*)&packet, packet.header.wLen, 0);
				}
			}
			//�����ʿ��� ����
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
		pInfo->len = 0;
		g_mapUser.insert(make_pair(client_sock, pInfo));

		PACKET_LOGIN_RET packet;
		packet.header.wIndex = PACKET_INDEX_FIRST_CONNECT;
		packet.header.wLen = sizeof(packet);
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
				//SendMessage(`hWnd, uMsg, wParam, lParam);
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
				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (!strcmp(loginPacket.ID, it->second->userID))
					{
						cout << "�̹� �α��� ��" << endl;
						loginRetPacket.header.wLen = sizeof(loginRetPacket);
						send(sock, (const char*)& loginPacket, header.wLen, 0);
						return false;
					}
				}
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
		packet.data.turn = !packet.data.turn;

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomIndex == packet.data.roomIndex)
			{
				send(iter->first, (const char*)&packet, header.wLen, 0);
			}
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
		for (auto it = g_RoomInfo[packet.roomIndex].begin(); it != g_RoomInfo[packet.roomIndex].end(); it++, i++)
		{
			strcpy(packet.playerIDs[i], (*it)->userID);
		}
		packet.playerNum = i;
		
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++, i++)
		{
			if(it->second->roomIndex == packet.roomIndex)
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
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = 6; //�׽�Ʈ��

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if(it->second->roomIndex == 0)
				send(it->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM: //���Ŀ� Ȯ�θ��ϰ� Ŭ�󿡼��� GET_PLAYERS �ѹ� �� ȣ���ϴ� �ɷ�.
	{
		PACKET_TRY_ENTER_THE_ROOM packetForLOBBY;
		PACKET_TRY_ENTER_THE_ROOM packetForRoom;
		memcpy(&packetForLOBBY, szBuf, header.wLen);
		memcpy(&packetForRoom, szBuf, header.wLen);

		//�������� ����(room)
		g_mapUser[sock]->roomIndex = packetForLOBBY.roomIndex;
		//�濡�߰�
		g_RoomInfo[packetForLOBBY.roomIndex].push_back(g_mapUser[sock]);

		int i = 0;

		//�κ񿡼� ����
		for(auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end();)
		{
			if (g_mapUser[sock]->userID == (*it)->userID)
			{
				it = g_RoomInfo[0].erase(it);
				break;
			}
			else
				it++;
		}
		//���� �� �������
		for (auto it = g_RoomInfo[packetForRoom.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.ID[i], (*it)->userID);
		}
		packetForRoom.userIndexInRoom = i - 1;
		packetForRoom.playerNum = i;
		
		i = 0;
		//�κ� �������
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.ID[i], (*it)->userID);
		}
		packetForLOBBY.userIndexInRoom = 0;
		packetForLOBBY.playerNum = i;

		packetForLOBBY.isSuccess = true;
		packetForRoom.isSuccess = true;

		//������ ������ ������ ������, ���ε��� ������ ����
		//�κ� ���� ����� ������, ���� ��
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
			{
				send(it->first, (const char*)&packetForLOBBY, header.wLen, 0);
				//g_RoomInfo[0];
			}
			else if (it->second->roomIndex == packetForRoom.roomIndex)
			{
				send(it->first, (const char*)&packetForRoom, header.wLen, 0);
			}
		}
	}
	break;
	case PACKET_INDEX_EXIT_THE_ROOM://���Ŀ� Ȯ�θ��ϰ� Ŭ�󿡼��� GET_PLAYERS, GET_ROOMS �ѹ� �� ȣ���ϴ� �ɷ�.
	{
		PACKET_TRY_EXIT_THE_ROOM packetForLOBBY;
		PACKET_TRY_EXIT_THE_ROOM packetForRoom;

		memcpy(&packetForLOBBY, szBuf, header.wLen);
		memcpy(&packetForRoom, szBuf, header.wLen);

		int i = 0;

		//�濡�� ����
		for (auto it = g_RoomInfo[packetForRoom.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomIndex].end();)
		{
			if (g_mapUser[sock]->userID == (*it)->userID)
			{
				it = g_RoomInfo[packetForRoom.roomIndex].erase(it);
				break;
			}
			else
				it++;
		}
		//�������� ����(room)
		g_mapUser[sock]->roomIndex = 0;

		//�κ��߰�
		g_RoomInfo[0].push_back(g_mapUser[sock]);

		//�κ� �������
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.ID[i], (*it)->userID);
		}
		packetForLOBBY.userIndexInRoom = i;
		packetForLOBBY.playerNum = i;

		i = 0;
		//������ ���
		for (auto it = g_RoomInfo[packetForRoom.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.ID[i], (*it)->userID);
		}
		packetForRoom.userIndexInRoom = i;
		packetForRoom.playerNum = i;
		
		packetForLOBBY.isSuccess = true;
		packetForRoom.isSuccess = true;

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
			{
				send(it->first, (const char*)& packetForLOBBY, header.wLen, 0);
				//g_RoomInfo[0];
			}
			else if (it->second->roomIndex == packetForRoom.roomIndex)
			{
				send(it->first, (const char*)& packetForRoom, header.wLen, 0);
			}
		}
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART blackstonePacket;
		PACKET_GAMESTART whitestonePacket;
		
		memcpy(&blackstonePacket, szBuf, header.wLen);
		memcpy(&whitestonePacket, szBuf, header.wLen);
		//retPacket.

		blackstonePacket.MyStone = 0;
		blackstonePacket.userIndexInRoom = 0;
		whitestonePacket.MyStone = 1;
		whitestonePacket.userIndexInRoom = 1;
		
		//�濡�ִ� �÷��̾� �鿡�� ���ӽ��� ������
		//0�� �������� �浹 �Ҵ�
		send(sock, (const char*)&blackstonePacket, header.wLen, 0);
		
		//�濡�� 1�� ���� ã��
		auto it = g_RoomInfo[g_mapUser[sock]->roomIndex].begin();
		it++;

		g_isGameplaying[g_mapUser[sock]->roomIndex] = true;
		//�鵹 1�� �������� �Ҵ�
		for (auto it2 = g_mapUser.begin(); it2 != g_mapUser.end(); it2++)
		{
			if (!strcmp((*it)->userID, it2->second->userID))
			{
				send(it2->first, (const char*)&whitestonePacket, header.wLen, 0);
				break;
			}
			//�κ� �����鿡�� �÷��� �� ���� �˸���
			/*if ((*it)->roomIndex == 0)
			{
				send(it2->first, (const char*)&whitestonePacket, header.wLen, 0);
			}*/
		}
		cout << g_mapUser[sock]->roomIndex << "�� �� ���� ����" << endl;
		//���� ���� ������ ����
	}
	break;
	case PACKET_INDEX_GAMEEXIT:
	{
		PACKET_GAMEEXIT packet;
		memcpy(&packet, szBuf, header.wLen);
		
		for (auto it = g_mapUser.begin(); it != g_mapUser.end();it++)
		{
			if (!strcmp(it->second->userID, packet.playerID))
			{
				g_mapUser.erase(it);
				break;
			}
		}
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++)
		{
			if (!strcmp((*it)->userID, packet.playerID))
			{
				g_RoomInfo[0].erase(it);
				break;
			}
		}

		int i = 0;
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packet.ID[i], (*it)->userID);
		}
		packet.playerNum = i;

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0) //�κ� �ο��鿡�Ը�
			{
				send(it->first, (const char*)& packet, header.wLen, 0);
			}
		}
		closesocket(sock);
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

DWORD __stdcall WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		DWORD cbTransferred;//���۷�
		SOCKET client_sock;

		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED *)&ptr, INFINITE);

		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;

				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				err_display((char *)"WSAGetOverlappedResult()");
			}
			closesocket(ptr->sock);
			printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			delete ptr;
			continue;
		}

		if (ptr->recvbytes == 0)
		{
			ptr->recvbytes = cbTransferred;
			ptr->sendbytes = 0;

			ptr->buf[ptr->recvbytes] = '\0';
			printf("[TCP \%s : %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
		}
		else
		{
			ptr->sendbytes += cbTransferred;
		}

		if (ptr->recvbytes > ptr->sendbytes)
		{
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					err_display((char *)"WSASend()");
				}
				continue;
			}
			else
			{
				ptr->recvbytes = 0;

				ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
				ptr->wsabuf.buf = ptr->buf;
				ptr->wsabuf.len = BUFSIZE;

				DWORD recvbytes;
				DWORD flags = 0;

				retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
				if (retval == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSA_IO_PENDING)
					{
						err_display((char *)"WSARecv()");
					}
					continue;
				}

			}
		}
		return 0;
	}

	return 0;
}