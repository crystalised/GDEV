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
	//GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, , 1.0f, 0);
	GetDevice()->BeginScene();

	RECT window = { 360, 180, 920, 540 };

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	RECT r = GetEngine()->GetWindowRect();
	D3DCOLOR inactive = WHITE_COL*0.5f;

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

	//DrawTransitionFade();	// fade, just add it before the EndScene
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}

//void ::Draw(float dt)
//{
//	GetDevice()->BeginScene();
//	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
//
//	POINT mouse = CGameWindow::GetMousePos();
//
//	RECT r = GetEngine()->GetWindowRect();
//	D3DCOLOR inactive = WHITE_COL*0.5f;
//
//	if (inAdvice)
//	{
//		DrawSprite(GetSprite(), mpQuestTex[1], r);
//
//		if (InSprite(mouse, CANCEL_BUT, mpButtonTex[5]))
//			DrawSprite(GetSprite(), mpButtonTex[5], CANCEL_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[5], CANCEL_BUT, inactive);
//	}
//	else if (inQuest)
//	{
//		DrawSprite(GetSprite(), mpQuestTex[2], r);
//
//		if (!questCompleted)
//		{
//			if (InSprite(mouse, ACCEPT_BUT, mpButtonTex[4]))
//				DrawSprite(GetSprite(), mpButtonTex[4], ACCEPT_BUT);
//			else
//				DrawSprite(GetSprite(), mpButtonTex[4], ACCEPT_BUT, inactive);
//		}
//
//		if (InSprite(mouse, CANCEL_BUT, mpButtonTex[5]))
//			DrawSprite(GetSprite(), mpButtonTex[5], CANCEL_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[5], CANCEL_BUT, inactive);
//	}
//	else
//	{
//		DrawSprite(GetSprite(), mpQuestTex[0], r);
//
//		if (InSprite(mouse, ADVISE_BUT, mpButtonTex[0]))
//			DrawSprite(GetSprite(), mpButtonTex[0], ADVISE_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[0], ADVISE_BUT, inactive);
//
//		if (InSprite(mouse, QUEST_BUT, mpButtonTex[1]))
//			DrawSprite(GetSprite(), mpButtonTex[1], QUEST_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[1], QUEST_BUT, inactive);
//
//		if (InSprite(mouse, SHOP_BUT, mpButtonTex[2]))
//			DrawSprite(GetSprite(), mpButtonTex[2], SHOP_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[2], SHOP_BUT, inactive);
//
//		if (InSprite(mouse, LEAVE_BUT, mpButtonTex[3]))
//			DrawSprite(GetSprite(), mpButtonTex[3], LEAVE_BUT);
//		else
//			DrawSprite(GetSprite(), mpButtonTex[3], LEAVE_BUT, inactive);
//	}
//
//	if (useMouse)
//		DrawSprite(GetSprite(), mpCursorTex, mouse, WHITE_COL);
//
//	GetSprite()->End();
//	GetDevice()->EndScene();
//}

void NPCScene::Leave()
{
	std::cout << "HelpScene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 2; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}