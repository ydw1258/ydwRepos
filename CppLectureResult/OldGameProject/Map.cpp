#include "Map.h"

void Map::LoadMap(int& index)
{
	string mapName[3];
	mapName[0] = "Debug/Town.txt";
	mapName[1] = "Debug/Map1.txt";

	//이미 로드된 맵은 재로드 할 필요 없음.
	if (LoadedMapIndex == index)
		return;
	LoadedMapIndex = index;

	ifstream inFile(mapName[index]);

	char inputString[100];
	map.clear();
	while (!inFile.eof()) {
		inFile.getline(inputString, 100);
		map.push_back(inputString);
	}
}

void Map::DrawMap(vector<GameObject*>* gameObject, int size)
{
	for (int i = 0; i < size; i++)
	{
		switch (gameObject->at(i)->objectType)
		{
		case PLAYER:
			map[gameObject->at(i)->y][gameObject->at(i)->x] = DRAW_PLAYER;
			break;
		case MONSTER:
			map[gameObject->at(i)->y][gameObject->at(i)->x] = DRAW_MONSTER;
			break;
		case NPC:
			map[gameObject->at(i)->y][gameObject->at(i)->x] = DRAW_NPC;
			break;
		case PORTAL:
			map[gameObject->at(i)->y][gameObject->at(i)->x] = DRAW_PORTAL;
			break;
		}
	}

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].length(); j++)
		{
			switch (map[i][j])
			{
			case DRAW_EMPTY:
				cout << "  ";
				break;
			case DRAW_WALL:
				cout << "■";
				break;
			case DRAW_PLAYER:
				cout << "☆";
				break;
			case DRAW_ITEM:
				cout << "♥";
				break;
			case DRAW_MONSTER:
				cout << "♣";
				break;
			case DRAW_NPC:
				cout << "◇";
				break;
			case DRAW_PORTAL:
				cout << "＠";
				break;
			}
		}
		cout << endl;
	}
}
Map::Map()
{

}

Map::~Map()
{
}