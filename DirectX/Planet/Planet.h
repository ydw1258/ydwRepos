#include<Windows.h>

class Planet
{
private:

public:
	void Init(HWND hWnd);
	void Render();
	void Update();
	void SetupMatrices();
	void Release();
	HRESULT InitVB();
	HRESULT InitIB();
	void DrawMesh();
};