#include "Scene.h"

const POINT LEFT_BUT = { 200, 260 };
const POINT RIGHT_BUT = { 460, 260 }; //Next, accept, done, teleport

/// the help scene.
/// displays instructions and a back button

void NPCScene::Enter()
{
	std::cout << "HelpScene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out
	// load the media
	//Quest scenes
	mpBackground[0] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat1.png"); //Intro
	mpBackground[1] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat2.png"); //Quest intro
	mpBackground[2] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat3.png"); //In progress
	mpBackground[3] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat4.png"); //Quest complete
	mpBackground[4] = LoadSpriteTex(GetDevice(), "../media/scene/NPCChat5.png"); //Tele to boss
	//Buttons / Controller
	mpButtonB = LoadSpriteTex(GetDevice(), "../media/scene/ButtonB.png"); //B Button
	mpButtonA = LoadSpriteTex(GetDevice(), "../media/scene/ButtonA.png"); //A Button
	//Keyboard
	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/scene/next.png"); //Next button (npcchat1)
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/scene/next2.png"); //Next button (npcchat1)  //highlighted
	mpButtonTex[2] = LoadSpriteTex(GetDevice(), "../media/scene/cancel.png"); //Cancel button (npcchat2, 4)
	mpButtonTex[3] = LoadSpriteTex(GetDevice(), "../media/scene/cancel2.png"); //Cancel button (npcchat2, 4) //highlighted
	mpButtonTex[4] = LoadSpriteTex(GetDevice(), "../media/scene/accept.png"); //Accept button (npcchat2, 4)
	mpButtonTex[5] = LoadSpriteTex(GetDevice(), "../media/scene/accept2.png"); //Accept button (npcchat2, 4) //Highlighted
	mpButtonTex[6] = LoadSpriteTex(GetDevice(), "../media/scene/done.png"); //Done button (npcchat3)
	mpButtonTex[7] = LoadSpriteTex(GetDevice(), "../media/scene/done2.png"); //Done button (npcchat3) //highlighted
	mpButtonTex[8] = LoadSpriteTex(GetDevice(), "../media/scene/teleport.png"); //Teleport button(npcchat5)
	mpButtonTex[9] = LoadSpriteTex(GetDevice(), "../media/scene/teleport2.png"); //Teleport button(npcchat5) //highlighted


	cogObtained = GameScene::GetCogObtained();
	inQuest = GameScene::GetQuestStatus();

	if (cogObtained == 50)
	{
		questComplete = true;
	}
	else
	{
		questComplete = false;
	}

	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();
	if (inQuest && cogObtained != 50)
		questStage = 3;
	else if (inQuest && cogObtained == 50)
		questStage = 4;
	else
		questStage = 0;

	leftClick = false;

	CGameWindow::SetMousePos(200, 260);
}
void NPCScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	mousepos = GetEngine()->GetMousePos();

	if (mpGamepad->IsGamepadConnected(0))
	{
		//DPAD RIGHT/LEFT
		if (mpGamepad->IsButtonPressed2(0, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			if (questStage == 0 || questStage == 1 || questStage == 2 || questStage == 3 || questStage == 4)
			{
				if (CGameWindow::GetMousePos().x == 200) //if at cancel
					CGameWindow::SetMousePos(460, 260); //set to accept
				else
					CGameWindow::SetMousePos(200, 260); //else set to cancel
			}
		}

		if (mpGamepad->IsButtonPressed3(0, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			if (questStage == 0 || questStage == 1 || questStage == 2 || questStage == 3 || questStage == 4)
			{
				if (CGameWindow::GetMousePos().x == 200) //if at cancel
					CGameWindow::SetMousePos(460, 260); //set to accept
				else
					CGameWindow::SetMousePos(200, 260); //else set to cancel
			}
		}

		//X
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_A))
			leftClick = true;

	}
	if (CGameWindow::KeyPress(VK_LBUTTON) || leftClick)
	{
		POINT mouse = CGameWindow::GetMousePos();
		if (questStage == 0)
		{
			if (InSprite(mouse, RIGHT_BUT, mpButtonTex[0]))	// clicked the button
			{
				questStage += 1;
			}
		}
		else if (questStage == 1)
		{
			if (InSprite(mouse, LEFT_BUT, mpButtonTex[1]))	// clicked the button
			{
				ExitScene();
			}
			if (InSprite(mouse, RIGHT_BUT, mpButtonTex[2]))	// clicked the button
			{
				inQuest = true;
				ExitScene();
			}
		}
		else if (questStage == 3)
		{
			if (InSprite(mouse, RIGHT_BUT, mpButtonTex[3]))	// clicked the button
			{
				ExitScene();
			}
		}
		else if (questStage == 4)
		{
			if (InSprite(mouse, LEFT_BUT, mpButtonTex[1]))	// clicked the button
			{
				ExitScene();
			}
			if (InSprite(mouse, RIGHT_BUT, mpButtonTex[2]))	// clicked the button
			{
				questStage += 1;
			}
		}
		else if (questStage == 5)
		{
			if (InSprite(mouse, RIGHT_BUT, mpButtonTex[4]))	// clicked the button
			{
				teleportToBoss = true;
				ExitScene();
			}
		}
		leftClick = false;
	}
}
void NPCScene::Draw(float dt)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BLACK_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	RECT window = GetEngine()->GetWindowRect();

	//stringstream sout;
	//sout << "Mouse pos X: " << mousepos.x;
	//sout << "Mouse pos Y: " << mousepos.y;
	//sout << "Cogs: " << cogObtained;

	//DrawD3DFont(mpFont, sout.str().c_str(), 880, 360, RED_COL);

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	POINT mouse = CGameWindow::GetMousePos();

	if (questStage == 0)
	{
		DrawSprite(GetSprite(), mpBackground[0], window);
		if (InSprite(mouse, RIGHT_BUT, mpButtonTex[0]))
			DrawSprite(GetSprite(), mpButtonTex[1], RIGHT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[0], RIGHT_BUT);
	}
	else if (questStage == 1)
	{
		DrawSprite(GetSprite(), mpBackground[1], window);
		if (InSprite(mouse, LEFT_BUT, mpButtonTex[2]))
			DrawSprite(GetSprite(), mpButtonTex[3], LEFT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[2], LEFT_BUT);

		if (InSprite(mouse, RIGHT_BUT, mpButtonTex[4]))
			DrawSprite(GetSprite(), mpButtonTex[5], RIGHT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[4], RIGHT_BUT);
	}
	else if (questStage == 3)
	{
		DrawSprite(GetSprite(), mpBackground[2], window);
		if (InSprite(mouse, RIGHT_BUT, mpButtonTex[6]))
			DrawSprite(GetSprite(), mpButtonTex[7], RIGHT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[6], RIGHT_BUT);
	}
	else if (questStage == 4)
	{
		DrawSprite(GetSprite(), mpBackground[3], window);
		if (InSprite(mouse, LEFT_BUT, mpButtonTex[2]))
			DrawSprite(GetSprite(), mpButtonTex[3], LEFT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[2], LEFT_BUT);
		if (InSprite(mouse, RIGHT_BUT, mpButtonTex[4]))
			DrawSprite(GetSprite(), mpButtonTex[5], RIGHT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[4], RIGHT_BUT);
	}
	else if (questStage == 5)
	{
		DrawSprite(GetSprite(), mpBackground[4], window);
		if (InSprite(mouse, RIGHT_BUT, mpButtonTex[8]))
			DrawSprite(GetSprite(), mpButtonTex[9], RIGHT_BUT); //highlighted
		else
			DrawSprite(GetSprite(), mpButtonTex[8], RIGHT_BUT);
	}

	GetSprite()->End();
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}

void NPCScene::Leave()
{
	std::cout << "HelpScene::Leave\n";
	SAFE_RELEASE(mpFont);
	for (int i = 0; i < 5; i++)
	{
		SAFE_RELEASE(mpButtonTex[i]);
	}
}

bool NPCScene::GetQuestStatus()
{
	return inQuest;
}

bool NPCScene::TeleToBoss()
{
	return teleportToBoss;
}