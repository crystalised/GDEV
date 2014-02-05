#include "Scene.h"

const POINT BACK_BUT = { 1100, 550 };

/// the help scene.
/// displays instructions and a back button

void NPCScene::Enter()
{
	std::cout << "HelpScene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out
	// load the media
	mpFont = CreateD3DFont(GetDevice(), "Arial", 24, true);
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/scene/Back1.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/scene/Back2.png"); //Highlighted
	mpBackground = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat.png"); //Background

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
}
void NPCScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_B))
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
void NPCScene::Draw(float dt)
{
	GetDevice()->BeginScene();

	RECT window = GetEngine()->GetWindowRect();

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	DrawSprite(GetSprite(), mpBackground, window);

	// you can draw fonts with scaling & other effects using DrawD3DFontEx
	// but it must have a spitebatch set with D3DXSPRITE_ALPHABLEND

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
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}

void NPCScene::Leave()
{
	std::cout << "HelpScene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}