#include<iostream>
#include<conio.h>
#include<ctime>
#include<Windows.h>
#include"GameObject.h"
#include"Player.h"
#include"Monster.h"
#include"SceneManager.h"
#include"Map.h"
#include"NPC.h"
#include"Portal.h"
using namespace std;

#define DEFAULTPLAYERHP 100;
#define DEFAULTPLAYERMP 100;
#define DEFAULTPLAYERX 0;
#define DEFAULTPLAYERY 5;
#define DEFAULTPLAYERATK 10;

int main()
{
	SceneManager sceneManager;

	Map map;
	int currentMapIndex = 0;
	Player player;
	NpcClass defaultNPC[5];
	Portal portal;
	Monster monster;
	vector<GameObject*> allCharacterObectVector;
	
	player.Init(1, 5, 100, 100, 10);
	defaultNPC[0].Init(3, 5, "마왕을 무찔러라\n");
	portal.Init(15, 3, 1, 1, 3);
	monster.Init(4, 4, 100, 100, 1);

	allCharacterObectVector.push_back(&player);
	allCharacterObectVector.push_back(&defaultNPC[0]);
	allCharacterObectVector.push_back(&portal);
	allCharacterObectVector.push_back(&monster);
	while (true)
	{
		switch (sceneManager.getSceneType())
		{
		case TITLE:
			sceneManager.setSceneType((SCENETYPE)sceneManager.TitleMenu());
			break;
		case TOWN:
			map.LoadMap(currentMapIndex);
			
			if (kbhit())
			{
				if (player.Input(getch(), &map, defaultNPC) == DRAW_PORTAL)
				{
					player.Init(portal.movingPlayerX, portal.movingPlayerY, player.hp, player.mp, player.atk);
					sceneManager.setSceneType(FIELD);
					currentMapIndex++;
					allCharacterObectVector.clear();
					allCharacterObectVector.push_back(&player);
				}
			}
				
			map.DrawMap(&allCharacterObectVector, allCharacterObectVector.size());
			player.Reading(defaultNPC[0]); //읽는 방법 변경 필요(여러 NPC에 호환되도록)
			break;
		case FIELD:
			map.LoadMap(currentMapIndex);
			if (kbhit())
			{
				if (player.Input(getch(), &map, defaultNPC) == DRAW_MONSTER)
				{
					sceneManager.setSceneType(BATTLE);
				}
			}
				
			map.DrawMap(&allCharacterObectVector, allCharacterObectVector.size());
			player.Reading(defaultNPC[0]);
			break;
		case BATTLE:
			cout << "배틀";
			//map.LoadMap(currentMapIndex);
			sceneManager.DrawBattleScene(player, monster);
			

			break;
		}
		system("cls");
	}
}