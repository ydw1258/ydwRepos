#pragma once

#define WIDTH 20
#define HEIGHT 20

class Map
{
private :
	int map[HEIGHT][WIDTH];
public:
	enum TILE
	{
		TILE_EMPTY,
		TILE_WALL,
		TILE_PLAYER
	};
	void DrawMap();
	Map();
	~Map();
	inline void SetPlayer(int x, int y)
	{
		map[y][x] = TILE_PLAYER;
	}
	inline void SetEmpty(int x, int y);
};

