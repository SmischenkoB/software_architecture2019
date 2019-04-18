
#include "stdafx.h"
#include "resource.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// returns TRUE when it is time tio quit the app
bool ProcessMessageLoop(double endTime)
{
	bool bQuit = false;
	MSG msg;
	LARGE_INTEGER time;
	int minProcessedMessages = 3;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		QueryPerformanceCounter(&time);
		if (msg.message == WM_QUIT)
			bQuit = true;
		else
		{
			minProcessedMessages--;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (minProcessedMessages < 0 && time.QuadPart >= endTime)
			break;
	}
	return bQuit;
}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// Get the desktop window size
	RECT desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), NULL, NULL, NULL,
		L"ShootingGame", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window, leaving a predefined offset to each margin
	HWND hWnd = CreateWindow(L"ShootingGame", L"Shooting Game - Ubisoft Bucharest Online Academy Support",
		WS_CAPTION | WS_SYSMENU, WINDOW_OFFSET_X, WINDOW_OFFSET_Y, desktop.right - 2 * WINDOW_OFFSET_X, desktop.bottom - 2 * WINDOW_OFFSET_Y,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(Engine::EngineInit(hWnd)))
	{
		// Create the game
		new Game();

		// Show the window
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		// Enter the message loop
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);

		LARGE_INTEGER StartingTime, EndingTime, TimeToFrame;
		TimeToFrame.QuadPart = 0;
		QueryPerformanceCounter(&StartingTime);

		while (ProcessMessageLoop(StartingTime.QuadPart + FRAME_TIME * Frequency.QuadPart - TimeToFrame.QuadPart) != true)
		{
			if (TimeToFrame.QuadPart >= FRAME_TIME * Frequency.QuadPart)
			{
				// Process frame operations
				float dt = (float)((double)TimeToFrame.QuadPart / (double)Frequency.QuadPart);

				if (dt > 0.1f) dt = 0.1f; // for debug operations, we limit dt to 1/10 sec

				// Be sure to have the latest Gamepad state, that you can poll in the game during the frame
				Input::UpdateInputState();
				// Run a game frame
				Game::g_pGame->RunFrame(dt);
				TimeToFrame.QuadPart = 0;
			}

			QueryPerformanceCounter(&EndingTime);
			TimeToFrame.QuadPart += (EndingTime.QuadPart - StartingTime.QuadPart);
			StartingTime.QuadPart = EndingTime.QuadPart;
		}

		// Clean-up game
		delete Game::g_pGame;
		Engine::EngineCleanup();
	}


	UnregisterClass(L"ShootingGame", wc.hInstance);
	return 0;
}



