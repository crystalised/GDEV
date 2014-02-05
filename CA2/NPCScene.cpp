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
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/scene/Cancel.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/scene/Next.png");
	mpButtonTex[2] = LoadSpriteTex(GetDevice(), "../media/scene/Accept.png");
	mpButtonTex[3] = LoadSpriteTex(GetDevice(), "../media/scene/Teleport.png");
	//Quest scenes
	mpBackground[0] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat1.png"); //Intro
	mpBackground[1] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat2.png");
	mpBackground[2] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat3.png");
	mpBackground[3] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat4.png");
	mpButtonB = LoadSpriteTex(GetDevice(), "../media/scene/ButtonB.png"); //B Button
	mpButtonA = LoadSpriteTex(GetDevice(), "../media/scene/ButtonA.png"); //A Button

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
	if (inQuest)
		questStage = 3;
	else
		questStage = 0;
}
void NPCScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	mousepos = GetEngine()->GetMousePos();

	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_B))
			ExitScene();
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_A))
			questStage += 1;

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
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BLACK_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	RECT window = GetEngine()->GetWindowRect();

	stringstream sout;
	sout << "Mouse pos X: " << mousepos.x;
	sout << "Mouse pos Y: " << mousepos.y;

	DrawD3DFont(mpFont, sout.str().c_str(), 880, 360, RED_COL);

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (questStage == 0)
		DrawSprite(GetSprite(), mpBackground[0], window);
	else if (questStage == 1)
		DrawSprite(GetSprite(), mpBackground[1], window);
	else if (questStage == 2)
	{
		inQuest = true;
		ExitScene();
	}

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