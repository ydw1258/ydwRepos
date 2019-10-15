#include "SockMgr.h"
#include "UserInfo.h"

SockMgr::SockMgr()
{
}

SockMgr::~SockMgr()
{
}

void SockMgr::AddNewPlayerInfo(SOCKET client_sock)
{
	UserInfo* pInfo = new UserInfo();
	pInfo->socket = client_sock;
	pInfo->recvLen = 0;
	memcpy(pInfo->id, "", IDPWSIZE);
	//id는 로그인 후 받아옴.
	pInfo->state = PS_LOGOUT;
	pInfo->roomNum = -1;
	userMap.insert(make_pair(client_sock, pInfo));
}

void SockMgr::RemovePlayer(SOCKET sock)
{
	delete userMap[sock];
	userMap.erase(sock);
	closesocket(sock);
}

void SockMgr::SendUserDataList()
{
	PACKET_USER_DATA user_packet;
	user_packet.header.wIndex = PACKET_INDEX_USER_DATA;
	user_packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(WORD) + sizeof(USER_DATA) * userMap.size();
	user_packet.wCount = userMap.size();
	int i = 0;
	for (auto iter = userMap.begin(); iter != userMap.end(); iter++, i++)
	{
		user_packet.data[i].state = iter->second->state;
		user_packet.data[i].roomNum = iter->second->roomNum;
		memcpy(user_packet.data[i].name, iter->second->id, IDPWSIZE);
	}

	for (auto iter = userMap.begin(); iter != userMap.end(); iter++, i++)
	{
		SendPacket(iter->first, user_packet, user_packet.header.wLen);
	}
}
