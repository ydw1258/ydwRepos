#pragma once
#include <map>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include "PACKET.h"

using namespace std;

class UserInfo;
class SockMgr
{
private:
	map<SOCKET, UserInfo*> userMap;

public:
	map<SOCKET, UserInfo*> getUserMap() { return userMap; }
	UserInfo* getUserInfo(SOCKET sock) { return userMap[sock]; }

public:
	SockMgr();
	~SockMgr();

	void AddNewPlayerInfo(SOCKET client_sock);
	void RemovePlayer(SOCKET sock);

	void SendUserDataList();

	template<typename T>
	void SendPacket(SOCKET sock, T packet, int len = -1);
};

template<typename T>
inline void SockMgr::SendPacket(SOCKET sock, T packet, int len)
{
	if (len == -1)
	{
		send(sock, (const char*)&packet, sizeof(packet), 0);
	}
	else
	{
		send(sock, (const char*)&packet, len, 0);
	}

	//return;
#if _DEBUG
	PACKET_HEADER header;
	memcpy(&header, (const char*)&packet, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_USER_DATA:
		cout << sock << ": PACKET_INDEX_USER_DATA ����." << endl;
		PACKET_USER_DATA data;
		cout << "wCount: " << data.wCount << endl;
		break;
	case PACKET_INDEX_TRY_LOGIN:
		cout << sock << ": PACKET_INDEX_TRY_LOGIN ����." << endl;
		break;
	case PACKET_INDEX_LOGIN_RESULT:
		cout << sock << ": PACKET_INDEX_LOGIN_RESULT ����." << endl;
		break;
	case PACKET_INDEX_TRY_JOIN_ROOM:
		cout << sock << ": PACKET_INDEX_TRY_JOIN_ROOM ����." << endl;
		break;
	case PACKET_INDEX_TRY_JOIN_ROOM_RESULT:
		cout << sock << ": PACKET_INDEX_TRY_JOIN_ROOM_RESULT ����." << endl;
		break;
	case PACKET_INDEX_JOIN_ROOM:
		cout << sock << ": PACKET_INDEX_JOIN_ROOM ����." << endl;
		break;
	case PACKET_INDEX_CHAT:
		cout << sock << ": PACKET_INDEX_CHAT ����." << endl;
		break;
	case PACKET_INDEX_PLAYER_LEAVE_ROOM:
		cout << sock << ": PACKET_INDEX_PLAYER_LEAVE_ROOM ����." << endl;
		break;
	case PACKET_INDEX_ROOM_INFO_REQ:
		cout << sock << ": PACKET_INDEX_ROOM_INFO_REQ ����." << endl;
		break;
	case PACKET_INDEX_ROOM_INFO:
		cout << sock << ": PACKET_INDEX_ROOM_INFO ����." << endl;
		break;
	default:
		cout << sock << ": ����. �˼����� ��Ŷ �ε���" << endl;
		break;
	}
#endif	
}
