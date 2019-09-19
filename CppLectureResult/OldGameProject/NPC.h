#pragma once
#include"GameObject.h"
#include<string>
using namespace std;

class NpcClass : public GameObject
{
private:
	string script;
public:
	void Init(int _x, int _y, string _script);
	string getScript() { return script; }
	NpcClass();
	~NpcClass();
};

