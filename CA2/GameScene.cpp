#pragma once
#include "Scene.h"
#include "SoundComponent.h"

#define M_PI 3.14159265358979323846
CSoundComponent* mpGameBGM;
CSoundComponent* mpGameSFX;

void GameScene::Enter()
{
	mpTerrain = new CTerrain(GetDevice(), "../media/Terrain/map.bmp", 5, 0.5);
	mpTerrain->GenerateTexture(D3DXVECTOR3(1, 1, 1));
	//Setup weather
	mpWeather = new CPrecipitation();
	mpWeather->Init(GetDevice(), "../media/Particle/Snowflake.png", D3DXVECTOR3(CParticleSystem::GetRandomFloat(.2f, .9f), -2, 0));
	// setup scene
	SetupDefaultD3DOptions(GetDevice(), true);
	// the models
	mpEnemyMesh = new CXMesh(GetDevice(), "../media/Model/Monster.x");
	mpPlayerMesh = new CXMesh(GetDevice(), "../media/Model/Player.x");
	mpFlameMesh = new CXMesh(GetDevice(), "../media/yellow_ball.x");
	mpRocketMesh = new CXMesh(GetDevice(), "../media/Model/Rocket.x");
	mpEnemyBulletMesh = new CXMesh(GetDevice(), "../media/Model/Bullet.x");
	mpBossMesh = new CXMesh(GetDevice(), "../media/Model/Boss.x");
	mpBossBulletMesh = new CXMesh(GetDevice(), "../media/Model/Bullet.x");
	mpNPCMesh = new CXMesh(GetDevice(), "../media/Model/Npc.x");
	mpCogWheelMesh = new CXMesh(GetDevice(), "../media/Model/Gear.x");
	mpHutMesh = new CXMesh(GetDevice(), "../media/Model/Hut.X");
	mpCrossHair = LoadSpriteTex(GetDevice(), "../media/crosshair.png");
	mpUI = LoadSpriteTex(GetDevice(), "../media/Scene/UI.png");
	mpMissileIcon = LoadSpriteTex(GetDevice(), "../media/Scene/Missile.png");
	mpFireIcon = LoadSpriteTex(GetDevice(), "../media/Scene/Flame.png");
	mpCogGUI = LoadSpriteTex(GetDevice(), "../media/Scene/Cogs.png");
	mpBossUI = LoadSpriteTex(GetDevice(), "../media/Scene/BossHp.png");

	mpGameBGM = GetEngine()->FindComponent<CSoundComponent>();
	mpGameSFX = GetEngine()->FindComponent<CSoundComponent>();
	mpGameBGM->PlayCue("GameBgm");

	//Setup player and camera
	mpPlayer = new Player(mpPlayerMesh, mpTerrain);
	mpPlayer->Init(GetDevice(), mpFlameMesh, mpRocketMesh);
	mCamera.SetHpr(D3DXVECTOR3(0.05, 2.35, 0.00));
	mpPlayer->mPlayer.SetHpr(mCamera.GetHpr());
	mCamera.mMode = 1;
	cogObtained = 0;
	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();

	//Setup enemies
	maxEnemies = 25;
	for (int x = 230; x < 350; x += randi(16, 20))
	{
		for (int z = 155; z < 275; z += randi(16, 20))
		{
			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain, GetDevice());
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(x, mpTerrain->GetHeight(x, z), z)));
			mpEnemy->mLife = 100;
			mEnemies.push_back(mpEnemy);
		}
	}

	//Setup boss
	mpBoss = new Boss(mpBossBulletMesh, mpTerrain, GetDevice());
	mpBoss->Init(mpBossMesh);
	mpBoss->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(210, mpTerrain->GetHeight(210, 60), 60)));
	mpBoss->mLife = 1500;
	bossActive = false;
	bossSpawnMinions = false;
	bossSpawned1 = false;
	bossSpawned2 = false;

	//Setup NPC
	mpNPC = new NPC();
	mpNPC->Init(mpNPCMesh);
	mpNPC->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(72.0f, mpTerrain->GetHeight(72.0f, 461.37f), 461.37f)));
	mpNPC->SetHpr(D3DXVECTOR3(-0.09f, 2.35f, 0.0f));

	mpHut = new NPC();
	mpHut->Init(mpHutMesh);
	mpHut->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(75.28f, mpTerrain->GetHeight(75.28f, 460.89f), 460.89f)));
	mpHut->SetHpr(D3DXVECTOR3(-0.03f, -7.10f, 0.0f));

	gameFont = CreateD3DFont(GetDevice(), "Segoe UI", 40, false);

	teleAvailable = false;

	inTown = true;
	tele_used_time = 0;
	teleportToBoss = false;
}

void GameScene::Update(float dt)
{
	CScene::Update(dt);	// VITAL

	//ShowCursor(false);

	if (IsTopMost() == false) //Pause scene if not top most
		return;

	//Player
	mpPlayer->Update(dt);
	mpPlayer->UpdateWeapon(dt);
	HandleInput(dt);
	if (mpPlayer->mPlayer.mLife <= 0) //show death scene when player is dead
	{
		teleportToBoss = false;
		bossActive = false;
		bossSpawned1 = false;
		bossSpawned2 = false;
		inQuest = false;
		ExitScene();
		GetEngine()->AddScene(new DeathScene());
		mpGameBGM->StopCue("GameBgm");
		mpGameSFX->PlayCue("GameoverSound");
	}

	//Quest
	inQuest = NPCScene::GetQuestStatus();

	if (!bossActive && inQuest)
		teleportToBoss = NPCScene::TeleToBoss();

	//Teleport
	if (clock() - tele_used_time > tele_CD)
		teleAvailable = false;
	else
		teleAvailable = true;

	//Update weather
	mpWeather->SetCentre(mCamera.GetPos());
	mpWeather->Update(dt);

	UpdateEnemies(mEnemies, &mpPlayer->mPlayer, dt);

	if (maxEnemies < 25)
	{
		int x = randi(230, 350);
		int z = randi(155, 275);

		mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain, GetDevice());
		mpEnemy->Init(mpEnemyMesh);
		mpEnemy->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(x, mpTerrain->GetHeight(x, z), z)));
		mpEnemy->mLife = 100;
		mEnemies.push_back(mpEnemy);

		maxEnemies += 1;
	}

	if (teleportToBoss)
	{
		mpPlayer->mPlayer.mLife = 100;
		teleportToBoss = false;
		bossActive = true;
		mpPlayer->mPlayer.SetPos(D3DXVECTOR3(210, mpTerrain->GetHeight(210, 90), 90));
		mpPlayer->mPlayer.SetHpr(D3DXVECTOR3(-0.16f, -3.14f, 0));
	}

	if (bossActive)
	{
		mpBoss->Update(dt, &mpPlayer->mPlayer);

		if (mpBoss->mLife <= 700 && bossSpawned1 == false)
		{
			bossSpawnMinions = true;
			bossSpawned1 = true;
		}

		if (mpBoss->mLife <= 400 && bossSpawned2 == false)
		{
			bossSpawnMinions = true;
			bossSpawned2 = true;
		}

		if (bossSpawnMinions)
		{
			D3DXVECTOR3 left = mpBoss->GetPos() + D3DXVECTOR3(-5, 0, 0);
			D3DXVECTOR3 right = mpBoss->GetPos() + D3DXVECTOR3(5, 0, 0);

			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain, GetDevice());
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(left));
			mpEnemy->mLife = 200;
			mEnemies.push_back(mpEnemy);

			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain, GetDevice());
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(right));
			mpEnemy->mLife = 200;
			mEnemies.push_back(mpEnemy);
			bossSpawnMinions = false;
		}

		if (mpBoss->mLife <= 0) //Show victory scene when boss is killed
		{
			teleportToBoss = false;
			bossActive = false;
			bossSpawned1 = false;
			bossSpawned2 = false;
			ExitScene();
			mpGameBGM->StopCue("GameBgm");
			GetEngine()->AddScene(new VictoryScene());
		}
	}

	for (int i = 0; i < mCog.size(); i++)
	{
		mCog[i]->Update(dt);
	}

	mpNPC->Update(&mpPlayer->mPlayer);

	CollisionCheck();

	mCamera.ChangeCameraMode(mCamera, mpPlayer->mPlayer);
}

void GameScene::Draw(float dt)
{
	mCamera.SetMatrixes(GetDevice());
	// Clear the backbuffer to a black color
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, SKY_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	// the terrain
	mpTerrain->Draw(IDENTITY_MAT, false);
	//Weather
	mpWeather->Draw(IDENTITY_MAT);

	//Enemies
	DrawEnemies(mEnemies);
	//Player
	mpPlayer->Draw();

	//Boss
	if (bossActive)
	{
		mpPlayer->flameThrowerLvl = 3;
		mpBoss->Draw();
		DrawMeshNodes(mpBoss->mBullets);
		mpBoss->bossPowerUp->Draw(IDENTITY_MAT);
	}

	//NPC
	mpNPC->Draw();

	//Huts
	mpHut->Draw();

	//Items
	DrawItems(mCog);

	//stringstream sout;
	//sout << "Player pos: " << mpPlayer->mPlayer.GetPos();
	//sout << "Player hpr: " << mpPlayer->mPlayer.GetHpr();
	//sout << "\nNo of enemies: " << mEnemies.size();
	//sout << "\nCurrent time: " << clock();
	//sout << "\nClock: " << clock();
	//sout << "\n DT: " << dt;
	//sout << "\n FPS: " << GetEngine()->GetFps();
	//sout << "\n Health: " << mpPlayer->mPlayer.mLife;
	//sout << "\n boss Active: " << bossActive;
	//sout << "\n Boss Health: " << mpBoss->mLife;
	//sout << "\n Cogs Available: " << cogObtained;
	//sout << "\n Tele to boss: " << teleportToBoss;
	//sout << "\n In Quest: " << inQuest;

	//DrawD3DFont(gameFont, sout.str().c_str(), 20, 20, RED_COL);

	//Display HP
	stringstream soutHP;
	soutHP << mpPlayer->mPlayer.mLife;
	DrawD3DFont(gameFont, soutHP.str().c_str(), 100, 23, BLACK_COL);
	DrawD3DFont(gameFont, soutHP.str().c_str(), 101, 24, WHITE_COL);

	//Display Missile Cooldown
	stringstream soutMCD;
	if (mpPlayer->rocketReady)
	{
		soutMCD << "Yes";
	}
	else
	{
		soutMCD << "No";
	}
	DrawD3DFont(gameFont, soutMCD.str().c_str(), 1129, 610, BLACK_COL);
	DrawD3DFont(gameFont, soutMCD.str().c_str(), 1130, 611, WHITE_COL);

	//Display Teleport Cooldown
	stringstream soutTCD;
	if (teleAvailable)
		soutTCD << "Yes"; //Temp value in place of the actual teleport cooldown
	else
		soutTCD << "No";
	DrawD3DFont(gameFont, soutTCD.str().c_str(), 1129, 663, BLACK_COL);
	DrawD3DFont(gameFont, soutTCD.str().c_str(), 1130, 664, WHITE_COL);

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	DrawSprite(GetSprite(), mpUI, 0, 0);
	DrawSprite(GetSprite(), mpCrossHair, 0, 0);

	//Display Cogs Obtained only when in quest
	if (bossActive)
	{
		stringstream soutBHP;
		soutBHP << mpBoss->mLife;
		DrawD3DFont(gameFont, soutBHP.str().c_str(), 780, 23, BLACK_COL);
		DrawD3DFont(gameFont, soutBHP.str().c_str(), 781, 24, WHITE_COL);
		DrawSprite(GetSprite(), mpBossUI, 0, 0);
	}

	if (inQuest && !bossActive)
	{
		stringstream soutCOGS;
		soutCOGS << cogObtained << " / 50";
		DrawD3DFont(gameFont, soutCOGS.str().c_str(), 1080, 24, BLACK_COL);
		DrawD3DFont(gameFont, soutCOGS.str().c_str(), 1081, 25, WHITE_COL);
		DrawSprite(GetSprite(), mpCogGUI, 0, 0);
	}

	if (mpPlayer->currentWeapon == 1)
	{
		DrawSprite(GetSprite(), mpFireIcon, 0, 0);
	}
	else
	{
		DrawSprite(GetSprite(), mpMissileIcon, 0, 0);
	}

	GetSprite()->End();

	GetDevice()->EndScene();
}

void GameScene::Leave()
{
	SAFE_DELETE(mpPlayer);
	SAFE_DELETE(mpTerrain);
	SAFE_DELETE(mpWeather);
	SAFE_DELETE(mpEnemyMesh);
	SAFE_DELETE(mpPlayerMesh);
	SAFE_DELETE(mpFlameMesh);
	SAFE_DELETE(mpEnemyBulletMesh);
	SAFE_DELETE(mpRocketMesh);
	SAFE_DELETE(mpBossBulletMesh);
	SAFE_DELETE(mpBossMesh);
	SAFE_DELETE(mpBoss);
	SAFE_DELETE(mpNPCMesh);
	SAFE_DELETE(mpNPC);
	SAFE_DELETE(mpCogWheelMesh);
	SAFE_DELETE(mpHutMesh);
	SAFE_DELETE(mpHut);

	//Delete all enemies
	for (int i = (int)mEnemies.size() - 1; i >= 0; i--)
	{
		delete mEnemies[i];
	}
	mEnemies.clear();

	//Delete all cog wheels(item)
	for (int i = (int)mCog.size() - 1; i >= 0; i--)
	{
		delete mCog[i];
	}
	mCog.clear();
}

void GameScene::CollisionCheck()
{
	//Collision check for weapon usage
	for (int en = 0; en < mEnemies.size(); en++)
	{
		for (int f = 0; f < mpPlayer->mFlames.size(); f++) //Flame thrower hitting enemy
		{
			if (CollisionMeshNode(mpPlayer->mFlames[f], mEnemies[en])) //Collision between flame and enemy
			{
				mpPlayer->mFlames[f]->Destroy();
				mEnemies[en]->Damage(mpPlayer->flameThrowerLvl);
				mpPlayer->mFlameThrower->AddParticle(mEnemies[en]->GetPos(), mEnemies[en]->flameDirection); //Simulate enemy on fire
			}
		}

		for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Rocket hitting enemy
		{
			if (CollisionMeshNode(mpPlayer->mRockets[b], mEnemies[en])) //Collision between rocket and enemy
			{
				mpPlayer->mRockets[b]->Destroy();
				mpGameSFX->PlayCue("Explosion");
				mEnemies[en]->Destroy();
				mpGameSFX->PlayCue("DeathSound");
				mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 3.0f), 30);
			}
		}
		for (int n = 0; n < mEnemies[en]->mBullets.size(); n++)
		{
			if (CollisionMeshNode(mEnemies[en]->mBullets[n], &mpPlayer->mPlayer, 1.0f))
			{
				mEnemies[en]->mBullets[n]->Destroy();
				mpPlayer->mPlayer.Damage(10);
			}
			DeleteDeadMeshNodes(mEnemies[en]->mBullets); //Delete dead enemy bullets
		}

		if (mEnemies[en]->mLife == 0 && !bossActive)
		{
			mpCogWheel = new Item();
			mpCogWheel->Init(mpCogWheelMesh);
			mpCogWheel->SetPos(mEnemies[en]->GetPos());
			mCog.push_back(mpCogWheel);
			maxEnemies -= 1;
		}
	}

	for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Rocket
	{
		if ((mpPlayer->mRockets[b]->GetPos().y) < (mpTerrain->GetHeight(mpPlayer->mRockets[b]->GetPos().x, mpPlayer->mRockets[b]->GetPos().z))) //collision between bomb and ground
		{
			mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 3.0f), 30);
			mpPlayer->mRockets[b]->Destroy();
			mpGameSFX->PlayCue("Explosion");
		}

		if (!mpPlayer->mRockets[b]->IsAlive()) //rocket boom in air
		{
			mpGameSFX->PlayCue("Explosion");
			mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(5.0f, 10.0f), 30);
		}

		if (CollisionMeshNode(mpPlayer->mRockets[b], mpHut)) //rocket and hut
		{
			mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 3.0f), 30);
			mpPlayer->mRockets[b]->Destroy();
			mpGameSFX->PlayCue("Explosion");
		}
	}

	//Item
	for (int i = 0; i < mCog.size(); i++)
	{
		if (CollisionMeshNode(&mpPlayer->mPlayer, mCog[i]))
		{
			cogObtained += mCog[i]->amount;
			mpGameSFX->PlayCue("Cogs");
			mCog[i]->Destroy();
		}
	}

	//Boss
	//Only check when it is active
	if (bossActive)
	{
		for (int f = 0; f < mpPlayer->mFlames.size(); f++) //Player's flame thrower
		{
			if (CollisionMeshNode(mpPlayer->mFlames[f], mpBoss))
			{
				mpPlayer->mFlames[f]->Destroy();
				mpBoss->Damage(mpPlayer->flameThrowerLvl);
				mpPlayer->mFlameThrower->AddParticle(mpBoss->GetPos(), mpBoss->flameDirection); //Simulate boss on fire
			}
		}

		for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Player's missle/rocket
		{
			if (CollisionMeshNode(mpPlayer->mRockets[b], mpBoss))
			{
				mpPlayer->mRockets[b]->Destroy();
				mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 2.5f));
				mpBoss->Damage(100);
			}
		}

		for (int a = 0; a < mpBoss->mBullets.size(); a++) //Boss's bullets
		{
			if (CollisionMeshNode(&mpPlayer->mPlayer, mpBoss->mBullets[a], 1.0f))
			{
				mpBoss->mBullets[a]->Destroy();
				mpPlayer->mPlayer.Damage(20);
			}
		}
		DeleteDeadMeshNodes(mpBoss->mBullets); //Delete destroyed boss bullets
	}

	//Collision between player and hut
	if (CollisionMeshNode(&mpPlayer->mPlayer, mpHut))
	{
		mpPlayer->mPlayer.SetPos(lastPos);
	}

	DeleteDeadMeshNodes(mpPlayer->mFlames); //Delete dead flame mesh
	DeleteDeadMeshNodes(mpPlayer->mRockets); //Delete dead bomb mesh
	DeleteDeadEnemies(mEnemies); //Delete dead enemies
	DeleteCollectedItems(mCog); //Delete collected items
}

void GameScene::TalkToNpc()
{
	if (mpNPC->inRange)
	{
		GetEngine()->AddScene(new NPCScene());
	}
}

void GameScene::HandleInput(float dt)
{
	lastPos = mpPlayer->mPlayer.GetPos();
	//Player
	D3DXVECTOR3 move; //Player move vector
	D3DXVECTOR3 turn = D3DXVECTOR3(0.f, 0.f, 0.f); //Turn vector

	//Debug
	if (CGameWindow::KeyPress(VK_F10))
		ExitScene();

	if (CGameWindow::KeyPress(VK_F8))
		teleportToBoss = true;

	if (CGameWindow::KeyPress(VK_F7))
		bossSpawnMinions = true;

	if (CGameWindow::KeyPress(VK_F6))
		cogObtained = 50;

	if (mpGamepad->IsGamepadConnected(0))
	{
		move = mpGamepad->GetVector(0, LEFT_X, NONE, LEFT_Y);
		turn = mpGamepad->GetVector(0, RIGHT_Y_INV, RIGHT_X, NONE) / 5; //reduce sensitivity
		if (mpGamepad->IsButtonPressed2(0, XINPUT_GAMEPAD_A))
			mpPlayer->Jump();
		if (mpGamepad->GetLeftTrigger(0) > 0)
			mpPlayer->SPEED = 20.0f;
		else
			mpPlayer->SPEED = 10.0f;
		if (mpGamepad->IsButtonPressed3(0, XINPUT_GAMEPAD_BACK))
		{
			if (mCamera.mMode == 1)
				mCamera.mMode = 0;
			else
				mCamera.mMode = 1;
		}

		if (mpGamepad->GetRightTrigger(0) > 0)
		{
			mpPlayer->Shoot();

			if (mpPlayer->currentWeapon == 1)
			{
				mpGameSFX->PlayCue("FlameSound");
			}

			if (mpPlayer->currentWeapon == 2)
			{
				if (mpPlayer->rocketReady == false)
				{
					mpGameSFX->PlayCue("MissileLaunch");
				}
			}
		}
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			if (mpPlayer->currentWeapon == 1)
				mpPlayer->currentWeapon = 2;
			else if (mpPlayer->currentWeapon == 2)
				mpPlayer->currentWeapon = 1;
		}

		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_Y))
		{
			TalkToNpc();
		}

		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_START))
		{
			if (clock() - tele_used_time > tele_CD)
			{
				if (inTown)
					mpPlayer->mPlayer.SetPos(D3DXVECTOR3(290, mpTerrain->GetHeight(290, 215), 215));
				else
					mpPlayer->mPlayer.SetPos(D3DXVECTOR3(70.0f, mpTerrain->GetHeight(70.0f, 464.0f), 464.0f));
				inTown = !inTown;
				tele_used_time = clock();
			}
		}
	}
	else
	{
		move = GetKeyboardVector(WSAD_KEYS);
		turn = GetMouseTurnVector(100, true);

		if (CGameWindow::KeyPress(VK_SPACE))
		{
			mpPlayer->Jump();
		}

		if (CGameWindow::KeyDown(VK_SHIFT))
		{
			mpPlayer->SPEED = 20.0f;
		}
		else
		{
			mpPlayer->SPEED = 10.0f;
		}

		if (CGameWindow::KeyPress('1'))
		{
			mpPlayer->ChangeWeapon(1);
		}
		else if (CGameWindow::KeyPress('2'))
		{
			mpPlayer->ChangeWeapon(2);
		}
		if (CGameWindow::KeyDown(VK_LBUTTON))
		{
			mpPlayer->Shoot();

			if (mpPlayer->currentWeapon == 1)
			{
				mpGameSFX->PlayCue("FlameSound");
			}

			if (mpPlayer->currentWeapon == 2)
			{
				if (mpPlayer->rocketReady == false)
				{
					mpGameSFX->PlayCue("MissileLaunch");
				}
			}
		}
		if (CGameWindow::KeyPress('Q'))
			TalkToNpc();
		if (CGameWindow::KeyPress('Y'))
		{
			if (clock() - tele_used_time > tele_CD && inQuest)
			{
				if (inTown)
					mpPlayer->mPlayer.SetPos(D3DXVECTOR3(290, mpTerrain->GetHeight(290, 215), 215));
				else
					mpPlayer->mPlayer.SetPos(D3DXVECTOR3(70.0f, mpTerrain->GetHeight(70.0f, 464.0f), 464.0f));
				inTown = !inTown;
				tele_used_time = clock();
			}
		}
	}

	mpPlayer->mPlayer.Move(move * dt * mpPlayer->SPEED);
	mpPlayer->mPlayer.Turn(turn * dt * mpPlayer->turn);

	//Limit player from looking past straight up or down
	float limit = (float)(M_PI / 2) - 0.01f;

	mpPlayer->mPlayer.mHpr.x = (float)__max(-limit, mpPlayer->mPlayer.mHpr.x);
	mpPlayer->mPlayer.mHpr.x = (float)__min(+limit, mpPlayer->mPlayer.mHpr.x);
}

int GameScene::GetCogObtained()
{
	return cogObtained;
}

bool GameScene::GetQuestStatus()
{
	return inQuest;
}