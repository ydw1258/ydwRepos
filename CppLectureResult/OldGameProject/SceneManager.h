#pragma once
#include<iostream>
#include<fstream>
#include<ctime>
#include<Windows.h>
#include"Map.h"
#include"Player.h"
#include"Monster.h"
using namespace std;

enum SCENETYPE
{
	TITLE = 0,
	TOWN,
	FIELD,
	BATTLE
};
class SceneManager
{
private:
	int selection;
	SCENETYPE scenetype;
	string stateLog[4] = { "a", "b", "c", "d"};
public:
	SCENETYPE getSceneType();
	void setSceneType(SCENETYPE _scenetype);
	void Intro();

	int TitleMenu();
	void DrawBattleScene(Player& player);
	void Drawstatelog();
	SceneManager();
	~SceneManager();

	void DrawBattleScene(Player& player, Monster& monster);
	void InBattleInput(Player& player, Monster& monster);
};