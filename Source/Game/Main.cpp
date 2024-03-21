#include "Common.h"
#include "Game/Game.h"
#include <stdio.h>

INT WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nCmdShow)
{
	//library::PrintHi();
	if (FAILED(library::InitWindow(hInstance, nCmdShow)))
	{
		return 0;
	}
	if (FAILED(library::InitDevice()))
	{
		library::CleanupDevice();
		return 0;
	}
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			library::Render();
		}
	}
	// Destroy
	library::CleanupDevice();
	return (int)msg.wParam;

}	