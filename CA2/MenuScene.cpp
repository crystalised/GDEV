#include "Scene.h"

const POINT PLAY_BUT = { 50, 550 },
HELP_BUT = { 400, 550 },
CREDIT_BUT = { 750, 550 },
EXIT_BUT = { 1100, 550 };

void MenuScene::Enter()
{
	std::cout << "MenuScene::enter\n";
	mTransOnTime = 2;	// slow fade in
	mTransOffTime = 0;	// slow fade out

	mpButtonTex[0] = LoadSpriteTex(GetDevice(), "../media/Scene/Play1.png");
	mpButtonTex[1] = LoadSpriteTex(GetDevice(), "../media/Scene/Play2.png"); //Highlighted
	mpButtonTex[2] = LoadSpriteTex(GetDevice(), "../media/Scene/Help1.png");
	mpButtonTex[3] = LoadSpriteTex(GetDevice(), "../media/Scene/Help2.png"); //Highlighted
	mpButtonTex[4] = LoadSpriteTex(GetDevice(), "../media/Scene/Credits1.png");
	mpButtonTex[5] = LoadSpriteTex(GetDevice(), "../media/Scene/Credits2.png"); //Highlighted
	mpButtonTex[6] = LoadSpriteTex(GetDevice(), "../media/Scene/Exit1.png");
	mpButtonTex[7] = LoadSpriteTex(GetDevice(), "../media/Scene/Exit2.png"); //Highlighted
	mpBGTex = LoadSpriteTex(GetDevice(), "../media/Scene/MainBG.png");
}
void MenuScene::Update(float dt)
{
	CScene::Update(dt);	// MUST CALL BASE CLASS'S Update
	if (IsTopMost() == false)	return;	// Pause if I'm not the topmost screen

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	// you will need to detect the mouse clicks
	// using InSprite() from SpriteUtils
	// to go to a new scene you will need to 
	// GetEngine()->AddScene(new TheSceneClass());
	// you will also need to call ExitScene() when you want the menu to close
	if (CGameWindow::KeyPress(VK_LBUTTON))
	{
		POINT mouse = CGameWindow::GetMousePos();
		if (InSprite(mouse, PLAY_BUT, mpButtonTex[0]))	// clicked the PLAY button
		{
			GetEngine()->AddScene(new GameScene);
		}
		else if (InSprite(mouse, HELP_BUT, mpButtonTex[1]))	// clicked the HELP button
		{
			GetEngine()->AddScene(new HelpScene);
		}
		else if (InSprite(mouse, CREDIT_BUT, mpButtonTex[2]))	// clicked the CREDIT button
		{
			GetEngine()->AddScene(new CreditScene);
		}
		else if (InSprite(mouse, EXIT_BUT, mpButtonTex[3]))	// clicked the EXIT button
		{
			ExitScene();
		}
	}

}
void MenuScene::Draw(float dt)
{
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BLACK_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	// textures must be within BeginScene()..EndScene
	// and within a SpriteBatches Begin..End
	// we need the D3DXSPRITE_ALPHABLEND to enable transparency
	// you need the background and three buttons
	// at PLAY_BUT,HELP_BUT & EXIT_BUT locations

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	// just a normal draw using the spritebatch
	DrawSprite(GetSprite(), mpBGTex, 0, 0); //Background
	
	POINT mouse = CGameWindow::GetMousePos();
	if (InSprite(mouse, PLAY_BUT, mpButtonTex[0])) //Highlighted the play button
	{
		DrawSprite(GetSprite(), mpButtonTex[1], PLAY_BUT);
	}
	else
	{
		DrawSprite(GetSprite(), mpButtonTex[0], PLAY_BUT);
	}

	if (InSprite(mouse, HELP_BUT, mpButtonTex[2])) //Highlighted the help button
	{
		DrawSprite(GetSprite(), mpButtonTex[3], HELP_BUT);
	}
	else
	{
		DrawSprite(GetSprite(), mpButtonTex[2], HELP_BUT);
	}

	if (InSprite(mouse, CREDIT_BUT, mpButtonTex[4])) //Highlighted the credits button
	{
		DrawSprite(GetSprite(), mpButtonTex[5], CREDIT_BUT);
	}
	else
	{
		DrawSprite(GetSprite(), mpButtonTex[4], CREDIT_BUT);
	}

	if (InSprite(mouse, EXIT_BUT, mpButtonTex[6])) //Highlighted the exit button
	{
		DrawSprite(GetSprite(), mpButtonTex[7], EXIT_BUT);
	}
	else
	{
		DrawSprite(GetSprite(), mpButtonTex[6], EXIT_BUT);
	}
	GetSprite()->End();

	DrawTransitionFade();// fade, just add it before the EndScene
	GetDevice()->EndScene();
	// note: no need to call Present, that is done in the CSceneEngine
}
void MenuScene::Leave()
{
	std::cout << "MenuScene::Leave\n";
	SAFE_RELEASE(mpBGTex);
	for (int i = 0; i<8; i++)
		SAFE_RELEASE(mpButtonTex[i]);
}