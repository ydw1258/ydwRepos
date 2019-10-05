#include "Camera.h"
Camera* Camera::mthis = nullptr;

void Camera::Render(HDC hdc)
{
	BitBlt(hdc, 0, 0, 1024, 720, ResourceManager::backBuffer->GetmemDC(), 0, 0, SRCCOPY);
}
