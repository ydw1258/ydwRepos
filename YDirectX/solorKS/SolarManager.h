#pragma once

class Mercury;
class Venus;
class Earth;
class Mars;
class Sun;
class Moon;
class SolarManager
{
private:
	SolarManager();
	~SolarManager();

	static SolarManager* pSolar;

public:
	static SolarManager* inst()
	{
		if (nullptr == pSolar)
		{
			pSolar = new SolarManager();
		}

		return pSolar;
	}

public:
	Mercury* pMercury;
	Venus* pVenus;
	Earth* pEarth;
	Mars* pMars;
	Sun* pSun;
	Moon* pMoon;

public:
	void init();
	void update();
	void release();

public:

};

