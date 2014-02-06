#include "Scene.h"
#include "SoundComponent.h"

const POINT MENU_BUT = { 1100, 550 };
CSoundComponent* mpVictorySFX;

/// the help scene.
/// displays instructions and a back button

void VictoryScene::Enter()
{
	std::cout << "Victory Scene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out

	// load the media
	mpFont = CreateD3DFont(GetDevice(), "Arial", 24, true);
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/Scene/Exit1.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/Scene/Exit2.png");
	mpBG = LoadSpriteTex(GetDevice(), "../media/scene/Win.png"); //Win background
	mpButtonB = LoadSpriteTex(GetDevice(), "../media/scene/ButtonB.png"); //B button

	mpVictorySFX = GetEngine()->FindComponent<CSoundComponent>();

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
}
void VictoryScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	// WARNING: you cannot have two calls to KeyPress for the same key within a function
	// the second call always gives false
	// therefore one KeyPress with many tests within it
	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_B))
			mpVictorySFX->PlayCue("ClickSound");
			ExitScene();
	}
	else
	{
		if (CGameWindow::KeyPress(VK_LBUTTON))
		{
			POINT mouse = CGameWindow::GetMousePos();
			if (InSprite(mouse, MENU_BUT, mpButtonTex[0]))	// clicked the menu button
			{
				mpVictorySFX->PlayCue("ClickSound");
				ExitScene();
			}
		}
	}
}
void VictoryScene::Draw(float dt)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, EMERALD_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	// you can draw fonts with scaling & other effects using DrawD3DFontEx
	// but it must have a spitebatch set with D3DXSPRITE_ALPHABLEND

	DrawSprite(GetSprite(), mpBG, GetEngine()->GetWindowRect());

	if (mpGamepad->IsGamepadConnected(0))
	{
		DrawSprite(GetSprite(), mpButtonB, MENU_BUT.x - 100, MENU_BUT.y + 30);
		DrawD3DFontEx(mpFont, GetSprite(), "Menu", D3DXVECTOR2(MENU_BUT.x, MENU_BUT.y + 50),
			RED_COL, D3DXVECTOR2(0, 0), 1.5f);
	}
	else
	{
		POINT mouse = CGameWindow::GetMousePos();
		if (InSprite(mouse, MENU_BUT, mpButtonTex[0])) //Highlighted the back button
		{
			DrawSprite(GetSprite(), mpButtonTex[1], MENU_BUT);
		}
		else
		{
			DrawSprite(GetSprite(), mpButtonTex[0], MENU_BUT);
		}
	}

	GetSprite()->End();

	DrawTransitionFade();	// fade, just add it before the EndScene
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}
void VictoryScene::Leave()
{
	std::cout << "Victory Scene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}