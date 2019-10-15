#include "LoginScene.h"
#include "ENUMHEADER.h"

#include "GameFrameWork.h"
#include "SceneMgr.h"
#include "BitMap.h"
#include "ResourceMgr.h"

#include "SHA1.h"
#include <fstream>

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

int LoginScene::Init(HWND hWnd, HINSTANCE hInstance)
{
	Scene::Init(hWnd, hInstance);

	bg = ResourceMgr::GetInstance()->GetBitmap("bg");

	if (idBox == nullptr)
	{
		idBox = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 362, 300, 300, 25, hWnd, (HMENU)HWND_ID_EDIT, hInstance, NULL);
		SendMessage(idBox, EM_SETLIMITTEXT, IDPWSIZE, 0);
	}
	else
	{
		ShowWindow(idBox, true);
	}

	if (pwBox == nullptr)
	{
		pwBox = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL | ES_PASSWORD, 362, 340, 300, 25, hWnd, (HMENU)HWND_PW_EDIT, hInstance, NULL);
		SendMessage(pwBox, EM_SETLIMITTEXT, IDPWSIZE, 0);
	}
	else
	{
		ShowWindow(pwBox, true);
	}

	return 0;
}

void LoginScene::Update(float deltatime)
{
	Input();
}

void LoginScene::Input()
{
	if (GetForegroundWindow() != hWnd)
		return;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (!returnDown)
		{
			returnDown = true;

			idString.reserve(IDPWSIZE);
			GetWindowTextA(idBox, (LPSTR)(idString.c_str()), IDPWSIZE);

			string pwString;
			pwString.reserve(IDPWSIZE);
			GetWindowTextA(pwBox, (LPSTR)(pwString.c_str()), IDPWSIZE);

			SendTryLogInPacket(idString, pwString);
		}
	}
	else if (returnDown)
	{
		returnDown = false;
	}

	if (GetKeyState(VK_TAB) & 0x8000)
	{
		if (!isTabDown)
		{
			isTabDown = true;

			if (GetFocus() == idBox)
			{
				cout << "pw" << endl;
				SetFocus(pwBox);
			}
			else
			{
				cout << "id" << endl;
				SetFocus(idBox);
			}
		}
	}
	else if (isTabDown)
	{
		isTabDown = false;
	}
}

void LoginScene::ProcessPacket(char * szBuf, int len)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RESULT:
	{
		cout << "LOGIN_RESULT 받음" << endl;

		PACKET_LOGIN_RESULT packet;
		memcpy(&packet, szBuf, header.wLen);

		if (packet.result == LR_SUCCESS)
		{
			cout << "로그인 성공" << endl;
			GameFrameWork::GetInstance()->SetMyId(idString.c_str());
			SceneMgr::GetInstance()->ChangeScene("LobbyScene");
		}
		else
		{
			cout << "로그인 실패" << endl;
			loginResult = packet.result;
		}
	}
	break;
	}
}

void LoginScene::Render(HDC hdc)
{
	bg->Draw(hdc, 0, 0);

	char text[128];

	TextOutA(hdc, 310, 303, "ID", strlen("ID"));
	TextOutA(hdc, 310, 345, "PW", strlen("PW"));

	if (loginResult == LR_FAIL_ALREADY_LOGGED_IN)
		sprintf(text, "이미 접속 중 입니다.");
	else if (loginResult == LR_FAIL_ID_NOT_FOUND)
		sprintf(text, "아이디를 찾을 수 없습니다.");
	else if (loginResult == LR_FAIL_WRONG_PW)
		sprintf(text, "비밀번호가 틀렸습니다.");
	else if (loginResult == LR_FAIL_DATA_MISSING)
		sprintf(text, "아이디와 비밀번호를 입력해 주세요.");
	else if (loginResult == LR_FAIL_MAX_USER)
		sprintf(text, "서버가 혼잡합니다.");
	else
		sprintf(text, "");

	SetTextAlign(hdc, TA_CENTER);
	TextOutA(hdc, 512, 500, text, strlen(text));
	SetTextAlign(hdc, TA_LEFT);
}

void LoginScene::Release()
{
	ShowWindow(idBox, false);
	ShowWindow(pwBox, false);
}

void LoginScene::SendTryLogInPacket(string id, string pw)
{
	if (strlen(id.c_str()) == 0 || strlen(pw.c_str()) == 0)
	{
		cout << "사용자가 아이디나 비밀번호를 입력하지 않음" << endl;
		loginResult = LR_FAIL_DATA_MISSING;
		return;
	}

	PACKET_TRY_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_TRY_LOGIN;
	packet.header.wLen = sizeof(packet);

	string temp = sha1(pw.c_str());

	memcpy(&packet.id, id.c_str(), IDPWSIZE);
	memcpy(&packet.pw, temp.c_str(), 64);

	send(GameFrameWork::GetInstance()->GetMySocket(),
		(const char*)&packet, sizeof(packet), 0);

	cout << "TRY_LOGIN 보냄: " << temp.c_str() << endl;

	//ofstream out("hash.txt");
	//out << "qq," << sha1("qq") << "\n";
	//out << "ww," << sha1("ww") << "\n";
	//out << "ee," << sha1("ee") << "\n";
	//out << "rr," << sha1("rr") << "\n";
	//out << "tt," << sha1("tt") << "\n";
	//out << "yy," << sha1("yy") << "\n";
	//out << "uu," << sha1("uu") << "\n";
	//out << "ii," << sha1("ii") << "\n";
	//out << "oo," << sha1("oo") << "\n";
	//out << "pp," << sha1("pp") << "\n";
	//out << "aa," << sha1("aa") << "\n";
	//out << "ss," << sha1("ss") << "\n";
	//out << "dd," << sha1("dd") << "\n";
	//out << "ff," << sha1("ff") << "\n";
	//out << "gg," << sha1("gg") << "\n";
	//out << "hh," << sha1("hh") << "\n";
	//out << "jj," << sha1("jj") << "\n";
	//out << "kk," << sha1("kk") << "\n";
	//out << "ll," << sha1("ll") << "\n";
	//out << "zz," << sha1("zz") << "\n";
	//out << "xx," << sha1("xx") << "\n";
	//out << "cc," << sha1("cc") << "\n";
	//out << "vv," << sha1("vv") << "\n";
	//out << "bb," << sha1("bb") << "\n";
	//out << "nn," << sha1("nn") << "\n";
	//out << "mm," << sha1("mm") << "\n";
	//out.close();
}
