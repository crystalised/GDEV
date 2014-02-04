#include "Scene.h"

const POINT BACK_BUT = { 1100, 550 };

/// the credit scene.
/// displays credits and a back button


void CreditScene::Enter()
{
	std::cout << "HelpScene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out
	// load the media
	mpFont = CreateD3DFont(GetDevice(), "Arial", 24, true);
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/scene/Back1.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/scene/Back2.png");

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
}
void CreditScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_B))
			ExitScene();
	}

	// WARNING: you cannot have two calls to KeyPress for the same key within a function
	// the second call always gives false
	// therefore one KeyPress with many tests within it
	if (CGameWindow::KeyPress(VK_LBUTTON))
	{
		POINT mouse = CGameWindow::GetMousePos();
		if (InSprite(mouse, BACK_BUT, mpButtonTex[0]))	// clicked the button
		{
			ExitScene();
		}
	}
}
void CreditScene::Draw(float dt)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CREDIT_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	// you can draw fonts with scaling & other effects using DrawD3DFontEx
	// but it must have a spitebatch set with D3DXSPRITE_ALPHABLEND
	DrawD3DFontEx(mpFont, GetSprite(), "Credits", D3DXVECTOR2(100, 0),
		ORANGE_COL, D3DXVECTOR2(0, 0), 2.0f);

	DrawD3DFontEx(mpFont, GetSprite(), "Creator - ", D3DXVECTOR2(100, 100),
		GOLD_COL, D3DXVECTOR2(0, 0), 1.0f);

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
void CreditScene::Leave()
{
	std::cout << "HelpScene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}