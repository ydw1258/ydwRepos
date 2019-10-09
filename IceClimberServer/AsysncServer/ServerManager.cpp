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
	//메모장에 저장된 계정정보 읽어오기
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

	//윈속초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	//입출력 완료포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return;

	//CPU개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//cpu개수*2 개의 작업자 스레드 생성
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
	
	//비동기로 구현됨
	client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		//cout << "accept()" <<endl;
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			goto ACCEPT_AGAIN;
		return true;
	}
	printf("[TCP서버] 클라이언트 접속: IP주소=%s, 포트번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	
	//소켓과 입출력 완료 포트 연결
	CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

	//소켓 정보 구조체 할당
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) 
		return true;

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->sock = client_sock;
	ptr->recvbytes = ptr->sendbytes = 0;
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = BUFSIZE;
	ServerManager::GetInstance()->InitUser(ptr->sock);

	// 비동기 입출력 시작
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
//클라가 보낸 패킷 읽기
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
		
		cout << "입력된 ID : " << loginPacket.ID << ", " <<
			"입력된 비밀번호 : "<< loginPacket.password << ", 실제비밀번호 : " << mapAccounts[loginPacket.ID] << endl;
		if (mapAccounts.find(loginPacket.ID) == mapAccounts.end()) {
			//로그인 실패
			loginPacket.isSuccess = false;
			cout << "로그인 실패" << endl;
		}
		else {
			//비번 틀림
			if (strcmp(mapAccounts[loginPacket.ID].substr(0, 10).c_str(), loginPacket.password))
			{
				loginPacket.isSuccess = false;
				cout << "비밀번호 틀림" << endl;
			}
			else
			{
				for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
				{
					if (!strcmp(loginPacket.ID, (const char*)it->second->playerID))
					{
						loginPacket.isSuccess = false;
						cout << "이미 로그인 중" << endl;
						
						send(sock, (const char*)&loginPacket, header.wLen, 0);
						return false;
					}
				}
				// 로그인 성공
				USER_INFO *user = new USER_INFO();
				//g_mapUser[sock]->userID = ;
				strcpy(user->playerID, loginPacket.ID);
				user->roomIndex = 0;
				g_mapUser[sock] = user;
				
				//로비에 유저 추가
				g_RoomInfo[0].push_back(user);
				loginPacket.playerIndexInroom = roomIndexes[0]++;
				
				/*int i = 0;
				for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
				{
					strcpy(loginPacket.IDs[i], (char*)(*it)->playerID);
				}
				loginPacket.playerNum = i;*/
				loginPacket.isSuccess = true;
				cout << "로그인 성공" << endl;
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
		
		cout << packet.data.chat << "온거" << endl;
		cout << packet.data.ID << "온거" << endl;
		//채팅한 유저의 방이 플레이 중이면
		if (g_isGameplaying[packet.data.roomIndex])
		{
			//정답체크
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

		//고정 방 6개
		for (int i = 0; i < ROOMNUM; i++)
		{
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = ROOMNUM; //테스트용

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
		g_mapUser[sock]->roomIndex = packetForLOBBY.roomInfo.roomIndex;
		//방에추가
		g_RoomInfo[packetForLOBBY.roomInfo.roomIndex].push_back(g_mapUser[sock]);

		int i = 0;

		//로비에서 제거
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
		//들어가는 방 유저목록
		for (auto it = g_RoomInfo[packetForRoom.roomInfo.roomIndex].begin(); it != g_RoomInfo[packetForRoom.roomInfo.roomIndex].end(); it++, i++)
		{
			strcpy(packetForRoom.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForRoom.userIndexInRoom = i - 1;
		packetForRoom.roomInfo.playerNum = i;

		i = 0;
		//로비 유저목록
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForLOBBY.userIndexInRoom = 0;
		packetForLOBBY.roomInfo.playerNum = i;

		//지워진 곳에는 지워진 내용을, 새로들어가는 곳에는 새로
		//로비 내용 지우고 보내기, 들어가는 방
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
	case PACKET_INDEX_EXIT_THE_ROOM://추후에 확인만하고 클라에서는 GET_PLAYERS, GET_ROOMS 한번 더 호출하는 걸로.
	{
		PACKET_TRY_EXIT_THE_ROOM packetForLOBBY;
		PACKET_TRY_EXIT_THE_ROOM packetForRoom;

		memcpy(&packetForLOBBY, szBuf, header.wLen);
		memcpy(&packetForRoom, szBuf, header.wLen);

		int i = 0;

		//방에서 제거
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
		//유저정보 변경(room)
		g_mapUser[sock]->roomIndex = 0;

		//로비에추가
		g_RoomInfo[0].push_back(g_mapUser[sock]);

		//로비 유저목록
		for (auto it = g_RoomInfo[0].begin(); it != g_RoomInfo[0].end(); it++, i++)
		{
			strcpy(packetForLOBBY.roomInfo.IDs[i], (*it)->playerID);
		}
		packetForLOBBY.userIndexInRoom = i;
		packetForLOBBY.roomInfo.playerNum = i;

		i = 0;
		//방유저 목록
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
		cout << g_mapUser[sock]->roomIndex << "번 방 게임 시작" << endl;
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

void ServerManager::ClientExit(SOCKET sock)
{
	//강제종료 유저
	PACKET_USERSLIST packet;
	packet.header.wIndex = PACKET_INDEX_GAMEEXIT;
	packet.header.wLen = sizeof(packet);
	packet.roomInfo.roomIndex = g_mapUser[sock]->roomIndex;

	//해당 방에서 제거
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

	//방 에서 유저 강종
	if (g_mapUser[sock]->roomIndex != 0)
	{
		PACKET_ROOMLIST packet;
		packet.header.wIndex = PACKET_INDEX_GET_ROOMS;
		packet.header.wLen = sizeof(packet);
		//고정 방 6개
		for (int i = 0; i < ROOMNUM; i++)
		{
			packet.roomIndex[i] = i;
			packet.playerNum[i] = g_RoomInfo[i + 1].size();
		}
		packet.NumOfRoom = ROOMNUM; //테스트용

		for (auto it = g_mapUser.begin(); it != g_mapUser.end(); it++)
		{
			if (it->second->roomIndex == 0)
				send(it->first, (const char*)& packet, packet.header.wLen, 0);
		}
	}
	//유저맵에서 제거
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
		//비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO* ptr;
		ServerManager::GetInstance()->retval = GetQueuedCompletionStatus(ServerManager::GetInstance()->hcp, &cbTransferred,
			(LPDWORD)& client_sock, (LPOVERLAPPED*)& ptr, INFINITE);

		// 클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)& clientaddr, &addrlen);
		
		//비동기 입출력 결과 확인
		if (ServerManager::GetInstance()->retval == 0 || cbTransferred == 0) {
			if (ServerManager::GetInstance()->retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				cout << "WSAGetOverlappedResult()" << endl;
			}
			//클라이언트 강제 종료
			ServerManager::GetInstance()->ClientExit(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n",inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			
			delete ptr;
			continue;
		}

		//데이터 전송량 갱신
		if (ptr->recvbytes == 0) {
			ptr->recvbytes = cbTransferred;
			ptr->sendbytes = 0;
			
			//받은 데이터 출력
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

			//데이터 받기
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
