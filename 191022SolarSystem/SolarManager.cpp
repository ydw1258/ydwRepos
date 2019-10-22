#include "SolarManager.h"

#include "Mercury.h"
#include "Venus.h"
#include "Earth.h"
#include "Mars.h"
#include "Sun.h"
#include "Moon.h"

SolarManager* SolarManager::pSolar = nullptr;

SolarManager::SolarManager()
{
}


SolarManager::~SolarManager()
{
}

void SolarManager::init()
{
	pMercury = new Mercury;
	pVenus = new Venus;
	pEarth = new Earth;
	pMars = new Mars;
	pSun = new Sun;
	pMoon = new Moon;

	pSun->set_pos(.0f, .0f, .0f);
	pSun->set_anglespeed(1 / 25.370f * 365.0f);

	pMercury->set_pos(4.0f, .0f, .0f);
	pMercury->set_anglespeed(1 / 10.4770f);
	pMercury->set_panglespeed(1 / 87.970f);
	pMercury->set_planet(pSun);

	pVenus->set_pos(7.0f, 0.0f, 0.0f);
	pVenus->set_anglespeed(1 / -58.3260f);
	pVenus->set_panglespeed(1 / 224.00f);
	pVenus->set_planet(pSun);

	pEarth->set_pos(10.0f, .0f, .0f);
	pEarth->set_anglespeed(1 / .2330f);
	pEarth->set_panglespeed(1 / 365.0f);
	pEarth->set_planet(pSun);

	pMoon->set_pos(3.0f, .0f, .0f);
	pMoon->set_anglespeed(.0f);
	pMoon->set_panglespeed(.0f);
	pMoon->set_planet(pEarth);

	pMars->set_pos(15.0f, .0f, .0f);
	pMars->set_anglespeed(1 / .24622962f);
	pMars->set_panglespeed(1 / 686.980f);
	pMars->set_planet(pSun);
}

void SolarManager::update()
{
	pSun->animate();
	pMercury->animate();
	pVenus->animate();
	pEarth->animate();
	pMoon->animate();
	pMars->animate();
}

void SolarManager::release()
{
	delete pMercury;
	delete pVenus;
	delete pEarth;
	delete pMoon;
	delete pMars;
	delete pSun;
}
