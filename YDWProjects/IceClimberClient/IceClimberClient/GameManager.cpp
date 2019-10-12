#include "GameManager.h"

using namespace std;

#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

GameManager * GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[7] = { "Resources\\UI\\blueBoard.bmp",
							"Resources\\UI\\lobbybackground.bmp",
							"Resources\\UI\\memo.bmp",
							"Resources\\UI\\UIButton.bmp",
	};
	ResourceManager::GetInstance()->Init(hdc, filename, 7);
	FontManager::Init();
	UIInit(hdc);
	RoomButtonsInit();
	PenInit();
	PacketManager::GetInstance()->InitConnection(hwnd);
	SceneInitiator();
}
//�� ��ȯ�� UI�� ���ġ
//�����ؾߵ� UI��
void GameManager::Draw(HDC hdc)
{
	DrawUI(hdc);
	switch (scene)
	{
	case LOBBY:
	{
		DrawChatWindow(hdc);
		DrawRooms(hdc);
		DrawCurUsers(hdc);
	}
	break;
	case ROOM_WAIT:
	{
		DrawCurUsers(hdc);
		DrawChatWindow(hdc);
		DrawPenColorsButton(hdc);
	}
	break;
	case PLAYING:
	{
		DrawCurUsers(hdc);
		DrawChatWindow(hdc);
		DrawPenColorsButton(hdc);
	}
	break;
	}
}
void GameManager::SceneInitiator()
{
	switch (scene)
	{
	case LOGIN:
		LOGINInput[0] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 300, 300, 300, 30, hwnd, (HMENU)0, hInstance, NULL);
		LOGINInput[1] = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 300, 350, 300, 30, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case LOBBY:
		chatInputBoxLobby = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 750, 800, 30, hwnd, (HMENU)0, hInstance, NULL);
		chatList.clear();
		//chatInputBox = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		break;
	case ROOM_WAIT:
		chatInputBoxInRoom = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		chatList.clear();
		break;
	case PLAYING:
		chatInputBoxInRoom = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 750, 750, 100, 20, hwnd, (HMENU)0, hInstance, NULL);
		chatList.clear();
		break;
	}
}
void GameManager::TimeCheck(float deltaTime)
{
	remainTime -= deltaTime;
}
void GameManager::TimeOver(HDC hdc)
{
	remainTime = TIME_LIMIT;
	//��ȭ�� �ʱ�ȭ
	whiteBoardImage.Init(hdc, 600, 600);
}
void GameManager::ReceiveChatStr(char* str)
{
	chatList.push_back(str);
}
void GameManager::RoomListInit(int NumOfRoom, int* playerNum)
{
	for (int i = 0; i < NumOfRoom; i++)
	{
		mapRoomPlayers[i] = playerNum[i];
	}
}
void GameManager::PlayerInit(int playerNum, char IDs[][10])
{
	vecPlayerID.clear();
	for (int i = 0; i < playerNum; i++)
	{
		vecPlayerID.push_back(IDs[i]);
	}
}
void GameManager::RoomButtonsInit()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < ROOMNUM / 2; j++)
		{
			UIButton button;
			char buf[128];
			sprintf(buf, "�� ��ȣ %d \n %d / %d", i * ROOMNUM / 2 + j + 1, mapRoomPlayers[i * ROOMNUM / 2 + j + 1], 4);
			button.Init(IMAGENUM_BLUEBOARD, 10 + j * 80, 10 + i * 80, 80, 80, buf);
			roomButtons.push_back(button);
		}
	}
}
void GameManager::RoomButtonUpdate()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < ROOMNUM / 2; j++)
		{
			UIButton button;
			char buf[128];
			sprintf(buf, "���ȣ %d \n %d / %d", i * ROOMNUM / 2 + j + 1, mapRoomPlayers[i * ROOMNUM / 2 + j], 4);
			button.Init(IMAGENUM_BLUEBOARD, 10 + j * 80, 10 + i * 80, 80, 80, buf);
			roomButtons.push_back(button);
		}
	}
}
void GameManager::UIInit(HDC hdc)
{
	blueboard.Init(IMAGENUM_BLUEBOARD, 1, 300, 111);
	lobbybackground.Init(IMAGENUM_LOBBYBACKGROUND, 1, 1200, 1000);
	memoImage.Init(IMAGENUM_MEMO, 1, 600, 1000);
	ButtonImage.Init(IMAGENUM_UIBUTTON, 1, 346, 173);
	startButton.Init(IMAGENUM_BLUEBOARD, 30, 700, 150, 50, (char *)"���� ����");
	roomExitButton.Init(IMAGENUM_BLUEBOARD, 230, 700, 150, 50, (char *)"������");
	//gameExitButton.Init(IMAGENUM_BLUEBOARD, 800, 800, 400, 200, (char *)"���� ����");
	whiteBoard = { 10, 100, 600, 600 };
	whiteBoardImage.Init(hdc, 600, 600);
}

void GameManager::GameOver()
{
	FontManager font;
	//��� ȭ��.
	//���� �� ���ھ� ���
	SceneChange(ROOM_WAIT);
	MessageBox(hwnd, "���� ��~", "><", MB_OK);
}
void GameManager::PenInit()
{
	for (int i = 0; i < PENCOLORNUM / 2 + 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			RECT pen = { 650 + 30 * j, 360 + i * 30 ,670 + 30 * j, 380 + i * 30 };
			lcolorPen.push_back(pen);
		}
	}
	penInfo[0] = { 0, 0, 0, 0 };     //blackPen;  
	penInfo[1] = { 0, 255, 255, 255 };   //whitePen;
	penInfo[2] = { 0, 255, 0, 0 };   //redPen;
	penInfo[3] = { 0, 0, 0, 255 };   //bluePen;
	penInfo[4] = { 0, 0, 255, 0 };   //greenPen;
	penInfo[5] = { 0, 255, 0, 255 };   //yellowPen;
	penInfo[6] = { 0, 0, 0, 128 };  //NavyPen;
	penInfo[7] = { 0, 0, 128, 128 };  //darkCyanPen;
	penInfo[8] = { 0, 128, 0, 0 };  //maroonPen;
	penInfo[9] = { 0, 128, 0, 128 };  	  //purplePen;
	penInfo[10] = { 0, 128, 128, 0 }; 	  //olivePen;
	penInfo[11] = { 0, 192, 192, 192 }; 	  //lightGreyPen;
	penInfo[12] = { 0, 128, 128, 128 }; 	  //darkGreyPen;
	penInfo[13] = { 0, 0, 255, 255 }; 	  //cyanPen;
	penInfo[14] = { 0, 0, 128, 0 }; 	  //darkGreenPen;
	penInfo[15] = { 0, 255, 165, 0 }; 	  //orangePen;
	penInfo[16] = { 0, 173,255,47 }; 	  //greenYellowPen;
}
void GameManager::PenColorButtonClickedCheck(POINT pt)
{
	int i = 0;
	for (auto it = lcolorPen.begin(); it != lcolorPen.end(); it++, i++)
	{
		if (Physics::GetInstance()->RECTbyPointCollisionCheck(*it, pt))
		{
			curPenInfo = penInfo[i];
			return;
		}
	}
	if (Physics::GetInstance()->RECTbyPointCollisionCheck(dumpButton, pt))
	{
		PacketManager::GetInstance()->SendDump(curPenInfo.r, curPenInfo.g, curPenInfo.b);
	}
	
}
void GameManager::EnterTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;
	bool flag = false;

	//i��°�� �ִ� ���ȣ
	for (auto it = roomButtons.begin(); it != roomButtons.end(); it++, i++)
	{
		if (it->isButtonClick(pt))
		{
			flag = true;
			break;
		}
	}
	if (flag == false || mapRoomPlayers[i] == 4)
		return;
	char buf[10];

	PacketManager::GetInstance()->SendEnterTheRoomPacket(i + 1);
}
void GameManager::ExitTheRoom(POINT pt)
{
	int i = 0;

	//i��°�� �ִ� ���ȣ
	if (roomExitButton.isButtonClick(pt))
	{
		PacketManager::GetInstance()->SendExitTheRoom();
	}
	char buf[10];
}
void GameManager::GameStart(POINT pt)
{
//	if (listPlayerID.size() < 2)
		//return;
	
	if (startButton.isButtonClick(pt))
	{
		if (PacketManager::GetInstance()->userIndexInRoom != 0)
		{
			MessageBox(hwnd, "����(0�� ����)�� ���� ����.", "?", MB_OK);
			return;
		}
		if (PacketManager::GetInstance()->isGameStart)
		{
			return;
		}
		else
		{
			PacketManager::GetInstance()->isGameStart = true;
		}
		//packet
		PacketManager::GetInstance()->SendGameStart();
	}
}
void GameManager::Login()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char buf[10];
	char buf2[10];
	GetWindowText(LOGINInput[0], buf, 10);
	GetWindowText(LOGINInput[1], buf2, 10);
	PacketManager::GetInstance()->SendLogin(buf, buf2);
}

void GameManager::SceneChange(SERVERMANAGER_SCENE _scene)
{
	switch (scene)
	{
	case LOGIN:
		DestroyWindow(LOGINInput[0]);
		DestroyWindow(LOGINInput[1]);
		break;
	case LOBBY:
		DestroyWindow(chatInputBoxLobby);
		break;
	case ROOM_WAIT:
		DestroyWindow(chatInputBoxInRoom);
		//ShowWindow(chatInputBoxIngame, false);
		break;
	case PLAYING:
		DestroyWindow(chatInputBoxInRoom);
	default:
		break;
	}
	scene = _scene;
	SceneInitiator();
}

void GameManager::InputChatting()
{
	if (GetForegroundWindow() != hwnd)
		return;
	char str[128];

	switch (scene)
	{
	case LOBBY:
	{
		GetWindowText(chatInputBoxLobby, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		PacketManager::GetInstance()->SendChattingData(str);
		SetWindowText(chatInputBoxLobby, "");
	}
	break;
	case ROOM_WAIT:
	case PLAYING:
	{
		GetWindowText(chatInputBoxInRoom, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		PacketManager::GetInstance()->SendChattingData(str);
		SetWindowText(chatInputBoxInRoom, "");
	}
	break;
	}
}
void GameManager::MouseButtonCheckInRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	ExitTheRoom(pt);
	GameStart(pt);
	//MouseDrawInGame(pt);
}

void GameManager::DrawChatWindow(HDC hdc)
{
	int i = 0;
	switch (scene)
	{
	case LOBBY:
		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			ChattingFont.Draw(hdc, (*it), 15, 50, 680 - 30 * i, "DungGeunMo", RGB(255, 255, 255));
		}
		break;
	case ROOM_WAIT:
	case PLAYING:
		Rectangle(hdc, 750, 400, 950, 700);

		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			ChattingFont.Draw(hdc, (*it), 15, 760, 680 - 30 * i, "DungGeunMo", RGB(0, 0, 0));
		}
		break;
	}
}
void GameManager::DrawUI(HDC hdc)
{
	switch (scene)
	{
	case LOGIN:
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		break;
	case LOBBY:
	{
		//Draw UI, Images
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		memoImage.DrawResizedObject(hdc, 600, 80, 400, 600);
		blueboard.DrawResizedObject(hdc, 10, 530, 700, 200);
	}
	break;
	case ROOM_WAIT:
	{
		FontManager playerInfoFont;
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		memoImage.DrawResizedObject(hdc, 640, 0, 400, 400);

		char buf[128];
		PacketManager::GetInstance()->userIndexInRoom;

		sprintf(buf, "%d ���� %d��° ����", PacketManager::GetInstance()->roomIndex, PacketManager::GetInstance()->userIndexInRoom);
		playerInfoFont.Draw(hdc, buf, 30, 720, 100, "DungGeunMo", RGB(255, 0, 0));
		roomExitButton.Draw(hdc, (char *)"DungGeunMo");
		startButton.Draw(hdc, (char *)"DungGeunMo");

		DrawPenColorsButton(hdc);
	}
	break;
	case PLAYING:
	{
		FontManager playerInfoFont;
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		memoImage.DrawResizedObject(hdc, 640, 0, 400, 400);

		char buf[128];
		PacketManager::GetInstance()->userIndexInRoom;

		sprintf(buf, "%d ���� %d��° ����", PacketManager::GetInstance()->roomIndex, PacketManager::GetInstance()->userIndexInRoom);
		playerInfoFont.Draw(hdc, buf, 30, 720, 100, "DungGeunMo", RGB(255, 0, 0));
		roomExitButton.Draw(hdc, (char *)"DungGeunMo");
		startButton.Draw(hdc, (char *)"DungGeunMo");

		FontManager remainTimeFont;
		whiteBoardImage.Draw(hdc, whiteBoard.left, whiteBoard.top, whiteBoard.right, whiteBoard.bottom);

		if (remainTime > 0)
			sprintf(buf, "���� �ð� : %d", (int)remainTime);
		else
		{
			sprintf(buf, "���� ���� �����");
			//�� ���̸� �׸��� ����
		}
		remainTimeFont.Draw(hdc, buf, 20, 30, 30, "DungGeunMo", RGB(255, 0, 0));

		FontManager CurTurnFont;
		sprintf(buf, "���� �� : %d �� ���� : %d", PacketManager::GetInstance()->curTurn, PacketManager::GetInstance()->score);

		//vecPlayerID.at(PacketManager::GetInstance()->curTurn)
		remainTimeFont.Draw(hdc, buf, 15, 200, 30, "DungGeunMo", RGB(255, 0, 0));

		sprintf(buf, "���� : %s", PacketManager::GetInstance()->answer);
		remainTimeFont.Draw(hdc, buf, 15, 500, 30, "DungGeunMo", RGB(255, 0, 0));

	}
	break;
	}
}

void GameManager::DrawCurUsers(HDC hdc)
{
	int i = 0;
	switch (scene)
	{
	case LOGIN:
		break;
	case LOBBY:
		playerInfoFont.Draw(hdc, "�κ� �÷��̾� ���", 20, 700, 260, "DungGeunMo", RGB(0, 0, 0));

		for (auto it = vecPlayerID.begin(); it != vecPlayerID.end(); it++, i++)
		{
			playerInfoFont.Draw(hdc, (*it), 15, 710, 300 + 30 * i, "DungGeunMo", RGB(0, 0, 0));
		}
		break;
	case ROOM_WAIT:
		playerInfoFont.Draw(hdc, "�ΰ��� �÷��̾� ���", 20, 730, 150, "DungGeunMo", RGB(0, 0, 0));
		
		for (auto it = vecPlayerID.begin(); it != vecPlayerID.end(); it++, i++)
		{
			//if(it->)
			playerInfoFont.Draw(hdc, (*it), 15, 730, 180 + 30 * i, "DungGeunMo", RGB(0, 0, 0));
		}
		break;
	default:
		break;
	}

}
void GameManager::DrawRooms(HDC hdc)
{
	for (auto it = roomButtons.begin(); it != roomButtons.end(); it++)
	{
		it->Draw(hdc, (char*)"DungGeunMo");
	}
}
void GameManager::whiteBoardDraw(DRAWPT drawPt, PenInfo penInfo)
{
	//���� rgb�� �޾Ƽ� ó�� �ؾ� ��
	pen = CreatePen(PS_SOLID, penInfo.penSize, RGB(penInfo.r, penInfo.g, penInfo.b));
	oPen = (HPEN)SelectObject(whiteBoardImage.GetmemDC(), pen);

	MoveToEx(whiteBoardImage.GetmemDC(), drawPt.startX - whiteBoard.left, drawPt.startY - whiteBoard.top, NULL);
	LineTo(whiteBoardImage.GetmemDC(), drawPt.endX - whiteBoard.left, drawPt.endY - whiteBoard.top);

	SelectObject(whiteBoardImage.GetmemDC(), oPen);
	DeleteObject(pen);
}

void GameManager::DrawPenColorsButton(HDC hdc)
{
	HBRUSH tempBrush, tempOBrush;

	for (int i = 0; i < PENCOLORNUM ; i++)
	{
		tempBrush = CreateSolidBrush(RGB(penInfo[i].r, penInfo[i].g, penInfo[i].b));
		tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
		Rectangle(hdc, lcolorPen.at(i).left, lcolorPen.at(i).top, lcolorPen.at(i).right, lcolorPen.at(i).bottom);
		DeleteObject(tempBrush);
	}
	tempBrush = CreateSolidBrush(RGB(curPenInfo.r, curPenInfo.g, curPenInfo.b));
	tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
	dumpButton = { 700, 700, 750, 750 };
	Rectangle(hdc, 700, 700, 750, 750);

	tempOBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, tempOBrush);
	DeleteObject(tempBrush);
}
void GameManager::DumpAll(int r, int g, int b)
{
	whiteBoardImage.Init(ResourceManager::GetInstance()->backBuffer->GetmemDC(), 600, 600, r, g, b);
}
void GameManager::penSizeAdjust(bool upDown)
{
	if(upDown)
		curPenInfo.penSize += 5;
	else if(!upDown && curPenInfo.penSize > 5)
		curPenInfo.penSize -= 5;
}
void GameManager::DrawPenCursur(HDC hdc, POINT pt)
{
	if (Physics::GetInstance()->RECTbyPointCollisionCheck(whiteBoard, pt) && scene == PLAYING)
	{
		HBRUSH tempBrush, tempOBrush;
		tempBrush = CreateSolidBrush(RGB(curPenInfo.r, curPenInfo.g, curPenInfo.b));
		tempOBrush = (HBRUSH)SelectObject(hdc, tempBrush);
		Ellipse(hdc, pt.x, pt.y, pt.x + curPenInfo.penSize, pt.y + curPenInfo.penSize);
		SelectObject(hdc, tempOBrush);
		DeleteObject(tempBrush);
	}
}
void GameManager::Release()
{
	lcolorPen.clear();
	chatList.clear();
	vecPlayerID.clear();
	roomButtons.clear();
	mapRoomPlayers.clear();
	PacketManager::GetInstance()->Release();
	ResourceManager::GetInstance()->Release();

	delete this;
}

/*
void GameManager::GameExit(POINT pt)
{
	int i = 0;

	if (roomExitButton.isButtonClick(pt))
	{
		return;
	}
	char buf[10];

	PacketManager::GetInstance()->SendGameExit(i + 1);
}*/
GameManager::~GameManager() {}