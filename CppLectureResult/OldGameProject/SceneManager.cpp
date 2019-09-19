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
	cout << "1. ���ӽ���" << endl;
	cout << "2. �ε�" << endl;
	cout << "3. �� ������" << endl;
	cout << "4. ����" << endl;
	
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
	cout << "~~~ ��Ʈ�� ~~~" << endl;
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
	cout << "-1. ���� " << endl;
	cout << "-2. ������ " << endl;
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
		stateLog[0] = "����!";
		break;
	case 2:
		for (int i = 0; i < 3; i++)
			stateLog[i + 1] = i;
		stateLog[0] = "������ ���";
		break;
	}
}
void SceneManager::Drawstatelog()
{
	for (int i = 0; i < 4; i++)
		cout << stateLog[i] << endl;
}