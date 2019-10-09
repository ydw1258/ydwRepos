#include "ServerManager.h"
#include <time.h>

ServerManager* ServerManager::mthis = nullptr;

ServerManager::ServerManager(){}
ServerManager::~ServerManager(){}
DWORD WINAPI WorkerThread(LPVOID arg);

#define _WINSOCKAPI_

void ServerManager::Init()
{
	srand(time(NULL));

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
	//�޸��忡 ����� �������� �о����
	ifstream answerFile("answer.txt");
	if (answerFile.is_open())
	{
		string line;
		while (getline(answerFile, line))
		{
			int idx = line.find("/n");
			//cout << line.substr(0, idx) << endl;
			answer.push_back(line.substr(0, idx));
		}
	}

	for (int i = 0; i < ROOMNUM; i++)
		roomIndexes[i] = 0;

	int retval;

	//�����ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	//����� �Ϸ���Ʈ ����
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return;

	//CPU���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//cpu����*2 ���� �۾��� ������ ����
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL)
			return;
		CloseHandle(hThread);
	}
	

	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		cout << "socket()" << endl;

	//
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
		cout << "ioctlsocket()" << endl;

	//bind()
	serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)& serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) 
		cout << "bind()" << endl;

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		cout << "listen()" << endl;
}

bool ServerManager::Update()
{
ACCEPT_AGAIN:
	TimerCheck();
	//accept()
	addrlen = sizeof(clientaddr);
	
	//�񵿱�� ������
	client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		//cout << "accept()" <<endl;
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			goto ACCEPT_AGAIN;
		return true;
	}
	printf("[TCP����] Ŭ���̾�Ʈ ����: IP�ּ�=%s, ��Ʈ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	
	//���ϰ� ����� �Ϸ� ��Ʈ ����
	CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

	//���� ���� ����ü �Ҵ�
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) 
		return true;

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->sock = client_sock;
	ptr->recvbytes = ptr->sendbytes = 0;
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = BUFSIZE;
	ServerManager::GetInstance()->InitUser(ptr->sock);

	// �񵿱� ����� ����
	flags = 0;
	retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes,
		&flags, &ptr->overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != ERROR_IO_PENDING) {
			cout << "WSARecv()" << endl;
		}
	}
	return false;
}

void ServerManager::TimerCheck()
{
	
	//timer Update
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	
	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();


	PACKET_TIMER packet;

	for (int i = 0; i < ROOMNUM; i++)
	{
		if (g_isGameplaying[i])
		{
			Timer[i] -= m_fElapseTime;
			packet.header.wIndex = PACKET_INDEX_TIMER;
			packet.header.wLen = sizeof(packet);
			packet.RemainTime = Timer[i];
			packet.isNextTurn = false;
			packet.isGameOver = false;
			if (Timer[i] < 0)
			{
				ROTATION++;
				if (ROTATION == MAX_ROTATION)
				{
					packet.isGameOver = true;
					g_isGameplaying[i] = false;
					return;
				}
					
				if (curTurn == g_RoomInfo.size())
					curTurn = 0;
				else
					curTurn++;
				Timer[i] = TIME_LIMIT;
				packet.isNextTurn = true;

				int answerIndex = rand() % answer.size();
				auto it = answer.begin();

				for (int i = 0; i < answerIndex; i++)
				{
					it++;
				}
				
				strcpy(packet.answer, it->c_str());
				packet.CurTurn = curTurn;

				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (it->second->roomIndex == i + 1)
						send(it->first, (const char*)& packet, packet.header.wLen, 0);
				}
			}
		}
	}
}
USER_INFO* ServerManager::GetUserInfo(SOCKETINFO* sock)
{
	return g_mapUser[sock->sock];
}
//Ŭ�� ���� ��Ŷ �б�
bool ServerManager::ProcessPacket(SOCKET sock, USER_INFO_STRING* pUser, char* szBuf, int& len)
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
		
		cout << "�Էµ� ID : " << loginPacket.ID << ", " <<
			"�Էµ� ��й�ȣ : "<< loginPacket.password << ", ������й�ȣ : " << mapAccounts[loginPacket.ID] << endl;
		if (mapAccounts.find(loginPacket.ID) == mapAccounts.end()) {
			//�α��� ����
			loginPacket.isSuccess = false;
			cout << "�α��� ����" << endl;
		}
		else {
			//��� Ʋ��
			if (strcmp(mapAccounts[loginPacket.ID].substr(0, 10).c_str(), loginPacket.password))
			{
				loginPacket.isSuccess = false;
				cout << "��й�ȣ Ʋ��" << endl;
			}
			else
			{
				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (!strcmp(loginPacket.ID, (const char*)it->second->playerID))
					{
						loginPacket.isSuccess = false;
						cout << "�̹� �α��� ��" << endl;
						
						send(sock, (const char*)&loginPacket, header.wLen, 0);
						return false;
					}
				}
				// �α��� ����
				USER_INFO *user = new USER_INFO();
				//g_mapUser[sock]->userID = ;
				strcpy(user->playerID, loginPacket.ID);
				user->roomIndex = 0;
				g_mapUser[sock] = user;
				
				//�κ� ���� �߰�
				g_RoomInfo[0].push_back(user);
				loginPacket.playerIndexInroom = roomIndexes[0]++;
				
				/*int i = 0;
				for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
				{
					strcpy(loginPacket.IDs[i], (char*)(*it)->playerID);
				}
				loginPacket.playerNum = i;*/
				loginPacket.isSuccess = true;
				cout << "�α��� ����" << endl;
			}
		}
		//loginPacket.header.wLen = sizeof(loginPacket);
		send(sock, (const char*)& loginPacket, header.wLen, 0);
	}
	break;
	case PACKET_INDEX_SEND_POS:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->second->roomIndex == packet.data.roomIndex)
			{
				send(iter->first, (const char*)&packet, header.wLen, 0);
			}
		}
	}
	break;
	case PACKET_INDEX_SEND_CHATTING:
	{
		PACKET_SEND_INGAME_DATA packet;
		memcpy(&packet, szBuf, header.wLen);

		char buf[128];
		sprintf(buf, "%s : %s", packet.data.ID, packet.data.chat);
		strcpy(packet.data.chat, buf);
		
		cout << packet.data.chat << "�°�" << endl;
		cout << packet.data.ID << "�°�" << endl;
		//ä���� ������ ���� �÷��� ���̸�
		if (g_isGameplaying[packet.data.roomIndex])
		{
			//����üũ
			if (strcmp(packet.data.chat, curAnswer))
			{
				packet.answerIsCorrect = true;
			}
		}
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
		packet.header.wIndex = PACKET_INDEX_GET_PLAYERS;
		int i = 0;
		for (auto it = g_RoomInfo[packet.roomInfo.roomIndex].begin(); it != g_RoomInfo[packet.roomInfo.roomIndex].end(); it++, i++)
		{
			strcpy(packet.roomInfo.IDs[i], (*it)->playerID);
		}
		packet.roomInfo.playerNum = i;

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++, i++)
		{
			if(it->second->roomIndex == packet.roomInfo.roomIndex)
				send(it->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_GET_ROOMS:
	{
		PACKET_ROOMLIST packet;
		memcpy(&packet, szBuf, header.wLen);

		//���� �� 6��
		for (int i = 0; i < ROOMNUM; i++)
		{
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = ROOMNUM; //�׽�Ʈ��

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
		g_mapUser[sock]->roomIndex = packetForLOBBY.roomInfo.roomIndex;
		//�濡�߰�
		g_RoomInfo[packetForLOBBY.roomInfo.roomIndex].push_back(g_mapUser[sock]);

		int i = 0;

		//�κ񿡼� ����
		for(auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end();)
		{
			if (g_mapUser[sock]->playerID == (*it)->playerID)
			{
				it = g_RoomInfo[0].erase(it);
				break;
			}
			else
				it++;
		}
		//���� �� �������
		for (auto it = g_RoomInfo[packetForRoom.roomInfo.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomInfo.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForRoom.userIndexInRoom = i - 1;
		packetForRoom.roomInfo.playerNum = i;

		i = 0;
		//�κ� �������
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForLOBBY.userIndexInRoom = 0;
		packetForLOBBY.roomInfo.playerNum = i;

		//������ ������ ������ ������, ���ε��� ������ ����
		//�κ� ���� ����� ������, ���� ��
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
			{
				send(it->first, (const char*)&packetForLOBBY, header.wLen, 0);
				//g_RoomInfo[0];
			}
			else if (it->second->roomIndex == packetForRoom.roomInfo.roomIndex)
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
		for (auto it = g_RoomInfo[packetForRoom.roomInfo.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomInfo.roomIndex].end();)
		{
			if (g_mapUser[sock]->playerID == (*it)->playerID)
			{
				it = g_RoomInfo[packetForRoom.roomInfo.roomIndex].erase(it);
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
			strcpy(packetForLOBBY.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForLOBBY.userIndexInRoom = i;
		packetForLOBBY.roomInfo.playerNum = i;

		i = 0;
		//������ ���
		for (auto it = g_RoomInfo[packetForRoom.roomInfo.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomInfo.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForRoom.userIndexInRoom = i;
		packetForRoom.roomInfo.playerNum = i;
		
		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
			{
				send(it->first, (const char*)& packetForLOBBY, header.wLen, 0);
				//g_RoomInfo[0];
			}
			else if (it->second->roomIndex == packetForRoom.roomInfo.roomIndex)
			{
				send(it->first, (const char*)& packetForRoom, header.wLen, 0);
			}
		}
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;

		memcpy(&packet, szBuf, header.wLen);
		g_isGameplaying[packet.roomInfo.roomIndex] = true;
		for (auto it2 = g_mapUser.begin(); it2 != g_mapUser.end(); it2++)
		{
			send(it2->first, (const char*)& packet, header.wLen, 0);
		}
		cout << g_mapUser[sock]->roomIndex << "�� �� ���� ����" << endl;
	}
	break;
	case PACKET_INDEX_GAMEEXIT:
	{
		PACKET_GAMEEXIT packet;
		memcpy(&packet, szBuf, header.wLen);

		for (auto it = g_mapUser.begin(); it != g_mapUser.end();it++)
		{
			if (!strcmp(it->second->playerID, packet.playerID))
			{
				g_mapUser.erase(it);
				break;
			}
		}
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++)
		{
			if (!strcmp((*it)->playerID, packet.playerID))
			{
				g_RoomInfo[0].erase(it);
				break;
			}
		}

		int i = 0;
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packet.roomInfo.IDs[i], (*it)->playerID);
		}
		packet.roomInfo.playerNum = i;

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

void ServerManager::ClientExit(SOCKET sock)
{
	//�������� ����
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
	packet.header.wLen = sizeof(packet);
	packet.roomInfo.roomIndex = g_mapUser[sock]->roomIndex;

	//�ش� �濡�� ����
	for (auto it = g_RoomInfo[g_mapUser[sock]->roomIndex].begin(); it != g_RoomInfo[g_mapUser[sock]->roomIndex].end(); it++)
	{
		if (!strcmp((*it)->playerID, g_mapUser[sock]->playerID))
		{
			g_RoomInfo[g_mapUser[sock]->roomIndex].erase(it);
			break;
		}
	}

	int i = 0;
	for (auto it = g_RoomInfo[g_mapUser[sock]->roomIndex].begin(); it != g_RoomInfo[g_mapUser[sock]->roomIndex].end(); it++, i++)
	{
		strcpy(packet.roomInfo.IDs[i], (*it)->playerID);
	}
	packet.roomInfo.playerNum = i;

	for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
	{
		if (it->second->roomIndex == g_mapUser[sock]->roomIndex)
		{
			send(it->first, (const char*)& packet, packet.header.wLen, 0);
		}
	}

	//�� ���� ���� ����
	if (g_mapUser[sock]->roomIndex != 0)
	{
		PACKET_ROOMLIST packet;
		packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
		packet.header.wLen = sizeof(packet);
		//���� �� 6��
		for (int i = 0; i < ROOMNUM; i++)
		{
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = ROOMNUM; //�׽�Ʈ��

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
				send(it->first, (const char*)& packet, packet.header.wLen, 0);
		}
	}
	//�����ʿ��� ����
	g_mapUser.erase(sock);
	closesocket(sock);
}

void ServerManager::InitUser(SOCKET sock)
{
	USER_INFO* userInfo = new USER_INFO();
	userInfo->len = 0;
	userInfo->roomIndex = 99;
	strcpy(userInfo->playerID, "?");
	
	g_mapUser[sock] = userInfo;
	//sdg_mapUser[sock]->szBuf = '\0';
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	ServerManager::GetInstance()->hcp = (HANDLE)arg;

	while (1)
	{
		//�񵿱� ����� �Ϸ� ��ٸ���
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO* ptr;
		ServerManager::GetInstance()->retval = GetQueuedCompletionStatus(ServerManager::GetInstance()->hcp, &cbTransferred,
			(LPDWORD)& client_sock, (LPOVERLAPPED*)& ptr, INFINITE);

		// Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)& clientaddr, &addrlen);
		
		//�񵿱� ����� ��� Ȯ��
		if (ServerManager::GetInstance()->retval == 0 || cbTransferred == 0) {
			if (ServerManager::GetInstance()->retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				cout << "WSAGetOverlappedResult()" << endl;
			}
			//Ŭ���̾�Ʈ ���� ����
			ServerManager::GetInstance()->ClientExit(ptr->sock);
			printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n",inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			
			delete ptr;
			continue;
		}

		//������ ���۷� ����
		if (ptr->recvbytes == 0) {
			ptr->recvbytes = cbTransferred;
			ptr->sendbytes = 0;
			
			//���� ������ ���
			ptr->buf[ptr->recvbytes] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), ptr->buf);

			USER_INFO_STRING pUser = { 0 };
			ServerManager::GetInstance()->ProcessPacket(ptr->sock, &pUser, ptr->buf, ptr->recvbytes);
		}
		else {
			ptr->sendbytes += cbTransferred;
		}

		if (ptr->recvbytes > ptr->sendbytes) {
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			ServerManager::GetInstance()->retval = WSASend(ptr->sock, &ptr->wsabuf, 1,
				&sendbytes, 0, &ptr->overlapped, NULL);
			if (ServerManager::GetInstance()->retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					cout << "WSASend()" << endl;
				}
				continue;
			}
		}
		else {
			ptr->recvbytes = 0;

			//������ �ޱ�
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			DWORD flags = 0;
			ServerManager::GetInstance()->retval = WSARecv(ptr->sock, &ptr->wsabuf, 1,
				&recvbytes, &flags, &ptr->overlapped, NULL);

			if (ServerManager::GetInstance()->retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					cout << "WSARecv()" << endl;
				}
				continue;
			}
		}
	}

	return 0;
}
