#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <vector>
#include <fstream>
#include <string>

#include "PACKET.h"
#include "UserInfo.h"
#include "SockMgr.h"

using namespace std;


#define SERVERPORT 9000
#define BUFSIZE 512

int userCount = 0;

map<string, string> idpwMap;

class ROOM_INFO
{
public:
	vector<UserInfo*> userVec;
	UserInfo* playerW = nullptr;
	UserInfo* playerB = nullptr;
};
ROOM_INFO roomArray[6];

struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

DWORD WINAPI WorkerThread(LPVOID arg);

void err_quit(const char *msg);
void err_display(const char *msg);

bool ProcessPacket(SOCKET sock, UserInfo* pUser, char* szBuf, int& len);
void LoadIdPwData();
LOGIN_RESULT TryLogIn(string id, string pw);
void PlayerLeave(SOCKET sock);
ROOM_JOIN_RESULT AddPlayerToRoom(UserInfo* player, int roomIdx);
void RemovePlayerFromRoom(UserInfo* player);

SockMgr* sockMgr;

int main(int argc, char *argv[])
{
	sockMgr = new SockMgr;
	LoadIdPwData();

	int retval;

	//윈속초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//입출력 완료포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	//CPU개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//cpu개수*2 개의 작업자 스레드 생성
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		printf("[TCP서버] 클라이언트 접속: IP주소=%s, 포트번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		sockMgr->AddNewPlayerInfo(client_sock);

		//소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		//소켓 정보 구조체 할당
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		ptr->recvbytes = ptr->sendbytes = 0;
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		// 비동기 입출력 시작
		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes,
			&flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR) {
			if (WSAGetLastError() != ERROR_IO_PENDING) {
				err_display("WSARecv()");
			}
			continue;
		}
	}

	WSACleanup();

	delete sockMgr;

	return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		//비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,
			(LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

		// 클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

		//비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0) {
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				err_display("WSAGetOverlappedResult()");
			}

			PlayerLeave(ptr->sock);

			closesocket(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
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

			UserInfo* pUser = sockMgr->getUserInfo(ptr->sock);
			ProcessPacket(ptr->sock, pUser, ptr->buf, ptr->recvbytes);
		}
		else {
			ptr->sendbytes += cbTransferred;
		}

		if (ptr->recvbytes > ptr->sendbytes) {
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1,
				&sendbytes, 0, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					err_display("WSASend()");
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
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1,
				&recvbytes, &flags, &ptr->overlapped, NULL);
			if (retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					err_display("WSARecv()");
				}
				continue;
			}
		}
	}

	return 0;
}

void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//=============================

bool ProcessPacket(SOCKET sock, UserInfo* pUser, char* szBuf, int& len)
{
	if (len > 0)
	{
		memcpy(&pUser->recvBuf[pUser->recvLen], szBuf, len);
		pUser->recvLen += len;
		len = 0;
	}

	if (pUser->recvLen < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, pUser->recvBuf, sizeof(header));

	if (pUser->recvLen < header.wLen)
		return false;

	cout << sock << ": " << header.wIndex << "받음" << endl;

	switch (header.wIndex)
	{
	case PACKET_INDEX_TRY_LOGIN:
	{
		PACKET_TRY_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);

		PACKET_LOGIN_RESULT resultPacket;
		resultPacket.header.wIndex = PACKET_INDEX_LOGIN_RESULT;
		resultPacket.header.wLen = sizeof(resultPacket);
		resultPacket.result = TryLogIn(packet.id, packet.pw);

		sockMgr->SendPacket(sock, resultPacket);

		if (resultPacket.result == LR_SUCCESS)
		{
			memcpy(sockMgr->getUserInfo(sock)->id, packet.id, IDPWSIZE);
			sockMgr->getUserInfo(sock)->state = PS_IN_LOBBY;

			sockMgr->SendUserDataList();
		}
		else
		{
			cout << "로그인 실패" << endl;
		}
	}
	break;
	case PACKET_INDEX_CHAT:
	{
		PACKET_CHAT packet;
		memcpy(&packet, szBuf, header.wLen);

		// 같은방에 있는 사람들에게
		if (sockMgr->getUserInfo(sock)->roomNum != -1)
		{
			for (auto p : roomArray[sockMgr->getUserInfo(sock)->roomNum].userVec)
			{
				sockMgr->SendPacket(p->socket, packet);
			}
		}
		// 
		else
		{
			for (auto p : sockMgr->getUserMap())
			{
				if (p.second->roomNum == -1)
				{
					sockMgr->SendPacket(p.second->socket, packet);
				}
			}
		}
	}
	break;
	}

	memcpy(&pUser->recvBuf, &pUser->recvBuf[header.wLen], pUser->recvLen - header.wLen);
	pUser->recvLen -= header.wLen;

	return true;
}

void LoadIdPwData()
{
	ifstream openFile("idpwData.txt");
	if (openFile.is_open())
	{
		string line;
		while (getline(openFile, line))
		{
			int idx = line.find(",");
			idpwMap.insert(
				make_pair(line.substr(0, idx), line.substr(idx + 1, line.length()))
			);
		}
		openFile.close();
	}
}

void SaveIdPwDataToText()
{
	ofstream out("idpwData.txt");
	if (out.is_open())
	{
		for (auto iter = idpwMap.begin(); iter != idpwMap.end(); ++iter)
		{
			out << (*iter).first << "," << (*iter).second << "\n";
		}
	}

	out.close();
}

LOGIN_RESULT TryLogIn(string id, string pw)
{
	if (userCount >= 20)
	{
		return LR_FAIL_MAX_USER;
	}

	if (idpwMap.find(id.c_str()) == idpwMap.end())
	{
		return LR_FAIL_ID_NOT_FOUND;
	}

	for (auto user : sockMgr->getUserMap())
	{
		if (!id.compare(user.second->id))
		{
			return LR_FAIL_ALREADY_LOGGED_IN;
		}
	}

	if (!idpwMap[id.c_str()].compare(pw.c_str()))
	{
		return LR_SUCCESS;
	}
	else
	{
		//cout << "입력됨: " << pw.c_str() << endl;
		//cout << "찾는값: " << idpwMap[id.c_str()] << endl;
		return LR_FAIL_WRONG_PW;
	}
}

void PlayerLeave(SOCKET sock)
{
	userCount--;

	// 방에 있었으면 처리
	RemovePlayerFromRoom(sockMgr->getUserInfo(sock));

	// 데이터 삭제
	sockMgr->RemovePlayer(sock);
	sockMgr->SendUserDataList();
}

ROOM_JOIN_RESULT AddPlayerToRoom(UserInfo* player, int roomIdx)
{
	if (roomArray[roomIdx].userVec.size() >= 20)
	{
		player->roomNum = -1;
		player->state = PS_IN_LOBBY;

		return RJS_FAIL;
	}

	roomArray[roomIdx].userVec.push_back(player);
	player->roomNum = roomIdx;

	//if (roomArray[roomIdx].playerW == nullptr)
	//{
	//	roomArray[roomIdx].playerW = player;
	//	player->state = PS_PLAY_W;

	//	return RJS_W;
	//}
	//else if (roomArray[roomIdx].playerB == nullptr)
	//{
	//	roomArray[roomIdx].playerB = player;
	//	player->state = PS_PLAY_B;

	//	return RJS_B;
	//}
	//else
	{
		player->state = PS_SPECTATOR;

		return RJS_SPECTATOR;
	}
}

void RemovePlayerFromRoom(UserInfo* player)
{
	if (player->roomNum == -1)
		return;

	vector<UserInfo*>& userVec = roomArray[player->roomNum].userVec;
	userVec.erase(find(userVec.begin(), userVec.end(), player));

	bool needToResetRoom = false;
	if (roomArray[player->roomNum].playerW == player)
	{
		roomArray[player->roomNum].playerW = nullptr;
		needToResetRoom = true;
	}
	else if (roomArray[player->roomNum].playerB == player)
	{
		roomArray[player->roomNum].playerB = nullptr;
		needToResetRoom = true;
	}

	// 같은방에 있던 사람들에 대한 처리
	PACKET_PLAYER_LEAVE_ROOM packet;
	packet.header.wIndex = PACKET_INDEX_PLAYER_LEAVE_ROOM;
	packet.header.wLen = sizeof(packet);
	memcpy(&packet.userName, player->id, IDPWSIZE);
	packet.roomIdx = player->roomNum;
	packet.destroyRoom = needToResetRoom;

	for (auto p : sockMgr->getUserMap())
	{
		if (p.second == player)
			continue;

		sockMgr->SendPacket(p.second->socket, packet);
	}

	if (needToResetRoom)
	{
		//othello->ResetBoard(player->roomNum);
	}

	player->roomNum = -1;
	player->state = PS_IN_LOBBY;
}