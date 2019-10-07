#include "IceClimberGameManager.h"

using namespace std;

#define IP_ADDRESS "127.0.0.1"
//"10.30.10.215"

GameManager * GameManager::mthis = nullptr;

void GameManager::Init(HDC hdc, HINSTANCE hInstance, HWND _hwnd)
{
	hwnd = _hwnd;
	string filename[7] = { "Resources\\UI\\blueBoard.bmp","Resources\\UI\\lobbybackground.bmp","Resources\\UI\\memo.bmp",
		"Resources\\UI\\UIButton.bmp",
	};
	ResourceManager::GetInstance()->Init(hdc, hInstance, filename, 7);

	blueboard.Init(IMAGENUM_BLUEBOARD, 1, 300, 111);
	lobbybackground.Init(IMAGENUM_LOBBYBACKGROUND, 1, 1200, 1000);
	memoImage.Init(IMAGENUM_MEMO, 1, 960, 640);
	ButtonImage.Init(IMAGENUM_UIBUTTON, 1, 346, 173);

	//방버튼
	for (int i = 0; i < ROOMNUM; i++)
	{
		UIButton button;
		char buf[128];
		sprintf(buf, "방번호 %d");
		button.Init(IMAGENUM_BLUEBOARD, 200, 200 + i * 400, 400, 400, buf);
		roomButtons.push_back(button);
	}
	gameExitButton.Init(IMAGENUM_BLUEBOARD, 800, 800, 400, 200, (char *)"게임 종료");
	startButton.Init(IMAGENUM_BLUEBOARD, 700, 700, 400, 200, (char *)"게임 시작");
	roomExitButton.Init(IMAGENUM_BLUEBOARD, 600, 600, 400, 200, (char *)"나가기");

	PacketManager::GetInstance()->InitConnection(hwnd);
	SceneInitiator();
}
//씬 전환시 UI들 재배치
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
	}
}

//수정해야될 UI들
void GameManager::Draw(HDC hdc)
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
		memoImage.DrawResizedObject(hdc, 650, 10, 300, 400);

		//Rectangle(hdc, gameExitButton.left, gameExitButton.top, gameExitButton.right, gameExitButton.bottom);
		DrawChatWindow(hdc);
		DrawRooms(hdc);
		DrawCurUsers(hdc);
	}
	break;
	case ROOM_WAIT:
	{
		lobbybackground.DrawResizedObject(hdc, 0, 0, 1000, 800);
		//font.Draw(playerID, 30, 800, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));

		DrawCurUsers(hdc);
		DrawChatWindow(hdc);

		char buf[128];
		PacketManager::GetInstance()->roomIndex;
		sprintf(buf, "%d 번방 %d번째 유저", PacketManager::GetInstance()->roomIndex, PacketManager::GetInstance()->userIndexInRoom);
		playerInfoFont.Draw(buf, 30, 720, 100, "Resources/oldgameFont.ttf", RGB(255, 0, 0));
		roomExitButton.Draw(hdc, (char *)"Resources/DungGeunMo.ttf");
		startButton.Draw(hdc, (char *)"Resources/DungGeunMo.ttf");
		
		//Rectangle(hdc, startButton.left, startButton.top, startButton.right, startButton.bottom);
		//Rectangle(hdc, gameExitButton.left, gameExitButton.top, gameExitButton.right, gameExitButton.bottom);
	}
	break;
	}
}
void GameManager::GameExit(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;

	if (roomExitButton.isButtonClick(pt))
	{
		return;
	}
	char buf[10];

	PacketManager::GetInstance()->SendGameExit(i + 1);
}
void GameManager::EnterTheRoom(POINT pt)
{
	if (GetForegroundWindow() != hwnd)
		return;

	int i = 0;
	bool flag = false;

	//i번째에 있는 방번호
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
	if (GetForegroundWindow() != hwnd)
		return;
	int i = 0;

	//i번째에 있는 방번호
	if (roomExitButton.isButtonClick(pt))
	{
		return;
	}
	char buf[10];

	PacketManager::GetInstance()->SendExitTheRoom();
}
void GameManager::GameStart(POINT pt)
{
	//if (listPlayerID.size() < 2)
	//		return;
	
	if (startButton.isButtonClick(pt))
	{
		if (PacketManager::GetInstance()->userIndexInRoom != 0)
		{
			MessageBox(hwnd, "방장(0번 유저)만 시작 가능.", "?", MB_OK);
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
	{
		GetWindowText(chatInputBoxInRoom, str, 128);
		//GetWindowText(chatInputBoxLobby, str, 128);
		PacketManager::GetInstance()->SendChattingData(str);
		SetWindowText(chatInputBoxInRoom, "");
	}
	break;
	}
}
void GameManager::MouseButtonCheckIngame(POINT pt)
{
	ExitTheRoom(pt);
	GameStart(pt);

	if (GetForegroundWindow() != hwnd)
		return;
}

void GameManager::DrawChatWindow(HDC hdc)
{
	int i = 0;

	switch (scene)
	{
	case LOBBY:
		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			ChattingFont.Draw((*it), 15, 50, 680 - 30 * i, "Resources\\DungGeunMo.ttf", RGB(255, 255, 255));
		}
		break;
	case ROOM_WAIT:
		Rectangle(hdc, 750, 400, 950, 700);

		for (auto it = chatList.rbegin(); it != chatList.rend(); it++, i++)
		{
			ChattingFont.Draw((*it), 15, 760, 680 - 30 * i, "Resources\\DungGeunMo.ttf", RGB(0, 0, 0));
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
		playerInfoFont.Draw("로비 플레이어 목록", 20, 710, 130, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			playerInfoFont.Draw((*it), 15, 710, 160 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
		}
		break;
	case ROOM_WAIT:
		playerInfoFont.Draw("인게임 플레이어 목록", 20, 730, 200, "Resources/oldgameFont.ttf", RGB(0, 0, 0));

		for (auto it = listPlayerID.begin(); it != listPlayerID.end(); it++, i++)
		{
			playerInfoFont.Draw((*it), 15, 730, 230 + 30 * i, "Resources/oldgameFont.ttf", RGB(0, 0, 0));
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
		it->Draw(hdc, (char*)"Resources\\DungGeunMo.ttf");
	}
}