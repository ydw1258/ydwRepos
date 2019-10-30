#include <d3d9.h>
#include<d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include "CUSTOM_DATATYPES.h"
#include "Transform.h"
#pragma comment(lib , "d3d9.lib")
#pragma comment(lib , "d3dx9.lib")
#pragma comment(lib , "winmm.lib")

class Planet : public Transform
{
private:

public:
	void Init(HWND hWnd);
	void Render();
	void Update();
	void SetupMatrices();
	void Release();
	//HRESULT InitVB();
	//HRESULT InitIB();
	void DrawMesh();
	Planet();
	~Planet();
};