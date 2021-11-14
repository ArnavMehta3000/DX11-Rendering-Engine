#include "Application.h"
#include"Engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*Application * theApp = new Application();

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}*/

	Engine::GetInstance(hInstance, nCmdShow);

	// Main message loop
	MSG msg = {0};

	while (WM_QUIT != msg.message && Engine::IsRunning())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*theApp->Update();
			theApp->Draw();*/
			Engine::GetInstance()->Update();
			Engine::GetInstance()->Render();
		}
	}

	Engine::GetInstance()->Clean();

	/*delete theApp;
	theApp = nullptr;*/

	return (int) msg.wParam;
}