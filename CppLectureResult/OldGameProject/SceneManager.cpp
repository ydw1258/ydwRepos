#include "SceneManager.h"

SceneManager::SceneManager()
{
	scenetype = TITLE;
	selection = 0;
}
SCENETYPE SceneManager::getSceneType()
{
	return scenetype;
}
void SceneManager::setSceneType(SCENETYPE _scenetype)
{
	scenetype = _scenetype;
}
SceneManager::~SceneManager()
{
}
int SceneManager::TitleMenu()
{
	cout << "1. 게임시작" << endl;
	cout << "2. 로드" << endl;
	cout << "3. 맵 에디터" << endl;
	cout << "4. 종료" << endl;
	
	int sel;
	cin >> sel;

	if (sel == 1)
		Intro();
	else if (sel == 4)
		exit(0);
	
	return sel;
}
void SceneManager::Intro()
{
	system("cls");
	cout << "~~~ 인트로 ~~~" << endl;
	Sleep(1000);
}

void SceneManager::DrawBattleScene(Player& player, Monster& monster)
{
	cout << "-----------------------------------------BATTLE-----------------------------------------" << endl;
	cout << "-PlayerHP : " << player.hp << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << "-PlayerMP : " << player.mp << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << "-PlayerATK : " << player.atk << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << endl;
	cout << "-MonsterHP : " << monster.hp << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << "-MonsterMP : " << monster.mp << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << "-MonsterATK : " << monster.atk << "\t\t\t\t\t\t\t\t\t\t\t   -" << endl;

	cout << "-Selection\t\t\t\t\t\t\t\t\t\t\t   -" << endl;
	cout << "-1. 공격 " << endl;
	cout << "-2. 아이템 " << endl;
	Drawstatelog();
	InBattleInput(player, monster);
}
void SceneManager::InBattleInput(Player& player, Monster& monster)
{
	int selection;
	cin >> selection;

	switch (selection)
	{
	case 1:
		monster.hp -= 10;
		player.mp -= 5;

		for (int i = 2; i > 0; i--)
			stateLog[i + 1] = i;
		stateLog[0] = "어택!";
		break;
	case 2:
		for (int i = 0; i < 3; i++)
			stateLog[i + 1] = i;
		stateLog[0] = "아이템 사용";
		break;
	}
}
void SceneManager::Drawstatelog()
{
	for (int i = 0; i < 4; i++)
		cout << stateLog[i] << endl;
}