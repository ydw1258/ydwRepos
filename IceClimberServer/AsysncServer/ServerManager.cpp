#include "ServerManager.h"
ServerManager* ServerManager::mthis = nullptr;

//쓰레드별 바이트 스트림 처리, 쓰레드가 있음에도 대기가 발생하면 CompletionPort 함수가 관리해줌
//쓰레드 생성은 미리해두고 쉬고 있는 쓰레드를 깨워 패킷을 받고 처리
//크리티컬 섹션 => 수정 되는 부분 (접근 동시 불허용) LOCK

ServerManager::ServerManager(){}
ServerManager::~ServerManager(){}
DWORD WINAPI WorkerThread(LPVOID arg);

void ServerManager::Init(HWND _hWnd)
{
	hWnd = _hWnd;

	//메모장에 저장된 계정정보 읽어오기
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
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	//입출력 완료 포트 생성
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return;

	//CPU 갯수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//작업자 스레드 생성 강의실 기준 16개 생성
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		//createThread 버그가 있어서 수정 필요
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

	DWORD cbTransferred;//전송량
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
		printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
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
			//강제종료 유저
			PACKET_USERSLIST packet;
			packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
			packet.header.wLen = sizeof(packet);
			packet.roomIndex = g_mapUser[wParam]->roomIndex;
			
			//해당 방에서 제거
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
			
			//방 에서 유저 강종
			if (g_mapUser[wParam]->roomIndex != 0)
			{
				PACKET_ROOMLIST packet;
				packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
				packet.header.wLen = sizeof(packet);
				//고정 방 6개
				for (int i = 0; i < 6; i++)
				{
					packet.roomIndex[i] = i;
					packet.playerNum[i] = g_RoomInfo[i + 1].size();
				}
				packet.NumOfRoom = 6; //테스트용

				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (it->second->roomIndex == 0)
						send(it->first, (const char*)&packet, packet.header.wLen, 0);
				}
			}
			//유저맵에서 제거
			g_mapUser.erase(wParam);
			closesocket(wParam);
		}
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	//연결 요청한 클라 받기
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
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on WSAAsyncSelect!!" << endl;
		}
		//연결 시도한 클라에게 index할당
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

		//수신한 패킷의 바이트 수
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
//클라가 보낸 패킷 읽기
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
		
		cout << "입력된 ID : " << loginPacket.ID << ", " <<
			"입력된 비밀번호 : "<< loginPacket.password << ", 실제비밀번호 : " << mapAccounts[loginPacket.ID] << endl;
		if (mapAccounts.find(loginPacket.ID) == mapAccounts.end()) {
			//로그인 실패
			cout << "로그인 실패" << endl;
		}
		else {
			//비번 틀림
			if (strcmp(mapAccounts[loginPacket.ID].c_str(), loginPacket.password))
			{
				//loginRetPacket.isLoginSuccess = false;
				//loginRetPacket.header.wIndex = PACKET_INDEX_LOGIN_RET;
				cout << "비밀번호 틀림" << endl;
			}
			// 로그인 성공
			else
			{
				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (!strcmp(loginPacket.ID, it->second->userID))
					{
						cout << "이미 로그인 중" << endl;
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

				//로비에 유저 추가
				// map<int, list<USER_INFO*>> g_RoomInfo; //0번 로비
				g_RoomInfo[0].push_back(user);
				loginPacket.roomIndex = roomIndexes[0]++;
				cout << "로그인 성공" << endl;
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
		
		//고정 방 6개
		for (int i = 0; i < 6; i++)
		{
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = 6; //테스트용

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if(it->second->roomIndex == 0)
				send(it->first, (const char*)&packet, header.wLen, 0);
		}
	}
	break;
	case PACKET_INDEX_ENTER_THE_ROOM: //추후에 확인만하고 클라에서는 GET_PLAYERS 한번 더 호출하는 걸로.
	{
		PACKET_TRY_ENTER_THE_ROOM packetForLOBBY;
		PACKET_TRY_ENTER_THE_ROOM packetForRoom;
		memcpy(&packetForLOBBY, szBuf, header.wLen);
		memcpy(&packetForRoom, szBuf, header.wLen);

		//유저정보 변경(room)
		g_mapUser[sock]->roomIndex = packetForLOBBY.roomIndex;
		//방에추가
		g_RoomInfo[packetForLOBBY.roomIndex].push_back(g_mapUser[sock]);

		int i = 0;

		//로비에서 제거
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
		//들어가는 방 유저목록
		for (auto it = g_RoomInfo[packetForRoom.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.ID[i], (*it)->userID);
		}
		packetForRoom.userIndexInRoom = i - 1;
		packetForRoom.playerNum = i;
		
		i = 0;
		//로비 유저목록
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.ID[i], (*it)->userID);
		}
		packetForLOBBY.userIndexInRoom = 0;
		packetForLOBBY.playerNum = i;

		packetForLOBBY.isSuccess = true;
		packetForRoom.isSuccess = true;

		//지워진 곳에는 지워진 내용을, 새로들어가는 곳에는 새로
		//로비 내용 지우고 보내기, 들어가는 방
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
	case PACKET_INDEX_EXIT_THE_ROOM://추후에 확인만하고 클라에서는 GET_PLAYERS, GET_ROOMS 한번 더 호출하는 걸로.
	{
		PACKET_TRY_EXIT_THE_ROOM packetForLOBBY;
		PACKET_TRY_EXIT_THE_ROOM packetForRoom;

		memcpy(&packetForLOBBY, szBuf, header.wLen);
		memcpy(&packetForRoom, szBuf, header.wLen);

		int i = 0;

		//방에서 제거
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
		//유저정보 변경(room)
		g_mapUser[sock]->roomIndex = 0;

		//로비에추가
		g_RoomInfo[0].push_back(g_mapUser[sock]);

		//로비 유저목록
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.ID[i], (*it)->userID);
		}
		packetForLOBBY.userIndexInRoom = i;
		packetForLOBBY.playerNum = i;

		i = 0;
		//방유저 목록
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
		
		//방에있는 플레이어 들에게 게임시작 보내기
		//0번 유저에게 흑돌 할당
		send(sock, (const char*)&blackstonePacket, header.wLen, 0);
		
		//방에서 1번 유저 찾기
		auto it = g_RoomInfo[g_mapUser[sock]->roomIndex].begin();
		it++;

		g_isGameplaying[g_mapUser[sock]->roomIndex] = true;
		//백돌 1번 유저에게 할당
		for (auto it2 = g_mapUser.begin(); it2 != g_mapUser.end(); it2++)
		{
			if (!strcmp((*it)->userID, it2->second->userID))
			{
				send(it2->first, (const char*)&whitestonePacket, header.wLen, 0);
				break;
			}
			//로비 유저들에게 플레이 중 임을 알리기
			/*if ((*it)->roomIndex == 0)
			{
				send(it2->first, (const char*)&whitestonePacket, header.wLen, 0);
			}*/
		}
		cout << g_mapUser[sock]->roomIndex << "번 방 게임 시작" << endl;
		//게임 중인 방으로 변경
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
			if (it->second->roomIndex == 0) //로비 인원들에게만
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
	printf("[오류]%s", lpMsgBuf);
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
		DWORD cbTransferred;//전송량
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
			printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
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