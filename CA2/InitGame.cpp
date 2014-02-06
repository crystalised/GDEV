#include "Scene.h"
#include "SoundComponent.h"

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
	eng.AddComponent(new CSoundComponent("../media/Audio/Audio.xgs", "../media/Audio/Audio.xwb", "../media/Audio/Audio.xsb"));
	return eng.Go(hInstance);	// runs everything
}