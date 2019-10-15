#include "GameFrameWork.h"

#include <math.h>
#include <stdio.h>

#include "SceneMgr.h"
#include "ResourceMgr.h"

#include "LoginScene.h"
#include "Player.h"

GameFrameWork* GameFrameWork::pThis = nullptr;

GameFrameWork::GameFrameWork()
{
	lastTime = std::chrono::system_clock::now();
}

GameFrameWork::~GameFrameWork()
{
}

void GameFrameWork::Init(HWND _hWnd, HINSTANCE hInstance, SOCKET sock)
{
	hWnd = _hWnd;
	mySock = sock;

	HDC hdc = GetDC(hWnd);

	ResourceMgr::GetInstance()->Init(hdc, hInstance);
	LoadResource();

	SceneMgr::GetInstance()->Init(hWnd, hInstance);
	SceneMgr::GetInstance()->InsertScene("Login", new LoginScene);
	SceneMgr::GetInstance()->ChangeScene("Login");

	memDC = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc, 1024, 768);
	oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

	ReleaseDC(hWnd, hdc);
}

void GameFrameWork::Release()
{
	ResourceMgr::GetInstance()->Release();
	SceneMgr::GetInstance()->Release();

	SelectObject(memDC, oldBitmap);
	DeleteObject(bitmap);
	DeleteDC(memDC);

	for (auto iter = userVec.begin(); iter != userVec.end(); iter++)
	{
		delete *iter;
	}
	userVec.clear();

	delete this;
}

void GameFrameWork::Update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - lastTime;
	//if (sec.count() < (1 / FPS))
	//	return;

	elapseTime = sec.count();
	lastTime = std::chrono::system_clock::now();

	SceneMgr::GetInstance()->Update(elapseTime);

	Render();
}

void GameFrameWork::Render()
{
	HDC hdc = GetDC(hWnd);
	SetBkMode(memDC, 1);

	SceneMgr::GetInstance()->Render(memDC);

	BitBlt(hdc, 0, 0, 1024, 768, memDC, 0, 0, SRCCOPY);

	/*TCHAR text[256];
	sprintf(text, "FPS: %d", (int)(1/elapseTime));
	TextOut(hdc, 420, 420, text, lstrlen(text));*/

	ReleaseDC(hWnd, hdc);
}

void GameFrameWork::ProcessPacket(char * szBuf, int len)
{
	memcpy(&packetBuf[myLen], szBuf, len);
	myLen += len;

	cout << "패킷받음." << len << endl;
	ProcessPacketBuf();
}

void GameFrameWork::ProcessPacketBuf()
{
	// 짧음
	if (myLen < sizeof(PACKET_HEADER))
	{
		cout << "패킷받음. 헤더보다 짧음" << endl;
		return;
	}

	PACKET_HEADER header;
	memcpy(&header, packetBuf, sizeof(header));

	if (myLen < header.wLen)
	{
		cout << "패킷받음. header.wLen보다 짧음" << endl;
		return;
	}

	// 긺
	if (myLen > header.wLen)
	{
		cout << "패킷받음. header.wLen보다 김." << endl;
		cout << "myLen: " << myLen << " / header.wLen: " << header.wLen
			<< "header.wIndex: " << header.wIndex << endl;
	}

	SceneMgr::GetInstance()->ProcessPacket(packetBuf, header.wLen);

	switch (header.wIndex)
	{
	case PACKET_INDEX_USER_DATA:
	{
		cout << "USER_DATA 받음" << endl;

		PACKET_USER_DATA packet;
		memcpy(&packet, packetBuf, header.wLen);

		for (auto iter = userVec.begin(); iter != userVec.end(); iter++)
		{
			delete *iter;
		}
		userVec.clear();

		for (int i = 0; i < packet.wCount; i++)
		{
			Player* pNew = new Player();
			pNew->myState = packet.data[i].state;
			memcpy(pNew->id, packet.data[i].name, IDPWSIZE);
			userVec.push_back(pNew);
		}
		cout << "usermap Size : " << userVec.size() << endl;
	}
	break;
	case PACKET_INDEX_TRY_JOIN_ROOM_RESULT:
	{
		cout << "TRY_JOIN_ROOM_RESULT 받음" << endl;

		PACKET_TRY_JOIN_ROOM_RESULT packet;
		memcpy(&packet, packetBuf, header.wLen);

/*		if (packet.result == RJS_W)
		{
			GameFrameWork::GetInstance()->playerType = PT_W;
		}
		else if (packet.result == RJS_B)
		{
			GameFrameWork::GetInstance()->playerType = PT_B;
		}
		else */if (packet.result == RJS_SPECTATOR)
		{
			GameFrameWork::GetInstance()->playerType = PT_SPECTATOR;
		}

		if (packet.result != RJS_FAIL)
		{
			GameFrameWork::GetInstance()->myRoom = packet.roomIdx;
			SceneMgr::GetInstance()->ChangeScene("InGame");
		}
	}
	break;
	}

	memcpy(&packetBuf, &packetBuf[header.wLen], myLen - header.wLen);
	myLen -= header.wLen;

	cout << "===" << endl;

	if (myLen >= sizeof(PACKET_HEADER))
	{
		cout << "남은것 다시 처리. myLen: " << myLen << endl;
		ProcessPacketBuf();
	}
}

void GameFrameWork::PrintUserList(HDC hdc)
{
	char text[128];
	int userCount = 0;

	for (auto user : userVec)
	{
		if (strlen(user->id) == 0)
			continue;

		sprintf(text, "%s", user->id);
		TextOutA(hdc, 800, 42 + (20 * userCount), text, strlen(text));
		userCount++;
	}
	sprintf(text, "접속자(%i):", userCount);
	TextOutA(hdc, 800, 22, text, strlen(text));
}

void GameFrameWork::LoadResource()
{
	ResourceMgr::GetInstance()->InsertBitmap("empty", L"Res\\empty.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("white", L"Res\\white_.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("black", L"Res\\black_.bmp");

	ResourceMgr::GetInstance()->InsertBitmap("roomBtn", L"Res\\roomBtn.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("bg", L"Res\\bg.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("blackWin", L"Res\\blackWin.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("whiteWin", L"Res\\whiteWin.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("draw", L"Res\\draw.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("button", L"Res\\button.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("chatbox", L"Res\\chatbox.bmp");
	ResourceMgr::GetInstance()->InsertBitmap("bar", L"Res\\bar.bmp");
}
