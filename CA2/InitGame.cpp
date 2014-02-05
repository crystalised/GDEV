#include "Scene.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	bool screenMode = false;
	//if (MessageBox(NULL, "Do you want to run in full screen?", "GameWindow", MB_YESNO) == IDYES)
	//{
	//	screenMode = true;
	//}

	ENABLE_LEAK_DETECTION();	// Enable run-time memory check for debug builds.
	CSceneEngine eng(1280, 720, screenMode);
	eng.AddComponent(new CGamepadComponent());
	eng.AddScene(new MenuScene());
	return eng.Go(hInstance);	// runs everything
}