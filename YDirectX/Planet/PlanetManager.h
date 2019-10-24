#pragma once
#include<vector>
#include"Planet.h"

using namespace std;

class PlanetManager
{
private:
	vector<Planet*> vPlanets;
public:
	PlanetManager();
	~PlanetManager();
};

