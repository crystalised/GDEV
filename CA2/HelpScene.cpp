#include "Scene.h"

const POINT BACK_BUT = { 1100, 550 };

/// the help scene.
/// displays instructions and a back button

void HelpScene::Enter()
{
	std::cout << "HelpScene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out
	// load the media
	mpFont = CreateD3DFont(GetDevice(), "Arial", 24, true);
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/scene/Back1.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/scene/Back2.png"); //Highlighted

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
}
void HelpScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	// WARNING: you cannot have two calls to KeyPress for the same key within a function
	// the second call always gives false
	// therefore one KeyPress with many tests within it
	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_B))
			ExitScene();
	}
	if (CGameWindow::KeyPress(VK_LBUTTON))
	{
		POINT mouse = CGameWindow::GetMousePos();
		if (InSprite(mouse, BACK_BUT, mpButtonTex[0]))	// clicked the button
		{
			ExitScene();
		}
	}
}
void HelpScene::Draw(float dt)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, HELP_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	// you can draw fonts with scaling & other effects using DrawD3DFontEx
	// but it must have a spitebatch set with D3DXSPRITE_ALPHABLEND
	DrawD3DFontEx(mpFont, GetSprite(), "Instructions", D3DXVECTOR2(100, 0),
		EMERALD_COL, D3DXVECTOR2(0, 0), 2.0f);

	DrawD3DFontEx(mpFont, GetSprite(), "Controls", D3DXVECTOR2(100, 50),
		EMERALD_COL, D3DXVECTOR2(0, 0), 1.5f);

	DrawD3DFontEx(mpFont, GetSprite(), "WASD - Move Player \nMouse - Move view (in first/third person view) \nLeft Click - Shoot \nF1-F5 - Switch views(Tracking, First Person, Third Person, Overhead Fixed, Top)", D3DXVECTOR2(100, 100),
		YELLOW_COL, D3DXVECTOR2(0, 0), 1.0f);

	DrawD3DFontEx(mpFont, GetSprite(), "Objectives", D3DXVECTOR2(100, 250),
		EMERALD_COL, D3DXVECTOR2(0, 0), 1.5f);

	DrawD3DFontEx(mpFont, GetSprite(), "Collect all the diamonds while navigating through the\nmaze before the time runs out!", D3DXVECTOR2(100, 300),
		YELLOW_COL, D3DXVECTOR2(0, 0), 1.0f);

	POINT mouse = CGameWindow::GetMousePos();
	if (InSprite(mouse, BACK_BUT, mpButtonTex[0])) //Highlighted the back button
	{
		DrawSprite(GetSprite(), mpButtonTex[1], BACK_BUT);
	}
	else
	{
		DrawSprite(GetSprite(), mpButtonTex[0], BACK_BUT);
	}

	GetSprite()->End();

	DrawTransitionFade();	// fade, just add it before the EndScene
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}
void HelpScene::Leave()
{
	std::cout << "HelpScene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}