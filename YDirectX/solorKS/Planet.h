#pragma once
#include <d3dx9.h>
#include "SolarCommon.h"

class Planet
{
public:
	Planet();
	~Planet();
private:
	float MyAngleSpeed;
	float MyPAngleSpeed;
	D3DXVECTOR3 MyPos;
	D3DXMATRIXA16	MyRotMat;
	D3DXMATRIXA16	MyPRotMat;
	D3DXMATRIXA16	MyTransMat;

	Planet* MyParent;
public:
	void set_pos(FLOAT _X, FLOAT _Y, FLOAT _Z)
	{
		MyPos.x = _X;
		MyPos.y = _Y;
		MyPos.z = _Z;
	}
	void set_anglespeed(FLOAT _VAlue)
	{
		MyAngleSpeed = _VAlue;
	}
	void set_panglespeed(FLOAT _VAlue)
	{
		MyPAngleSpeed = _VAlue;
	}

	void set_planet(Planet* _Parent)
	{
		MyParent = _Parent;
	}

public:
	D3DXMATRIXA16	MyFinalMat;
	

	virtual void animate();
};