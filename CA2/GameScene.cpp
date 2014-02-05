#pragma once
#include "Scene.h"

#define M_PI 3.14159265358979323846

void GameScene::Enter()
{
	mpTerrain = new CTerrain(GetDevice(), "../media/Terrain/map.bmp", 5, 0.5);
	mpTerrain->GenerateTexture(D3DXVECTOR3(0.5f, 1, 0));
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
	mpNPCMesh = new CXMesh(GetDevice(), "../media/Model/NPC.x");
	mpCogWheelMesh = new CXMesh(GetDevice(), "../media/Model/Gear.x");
	mpCrossHair = LoadSpriteTex(GetDevice(), "../media/crosshair.png");

	//Setup player and camera
	mpPlayer = new Player(mpPlayerMesh, mpTerrain, GetEngine());
	mpPlayer->Init(GetDevice(), mpFlameMesh, mpRocketMesh);
	mpPlayer->mPlayer.SetHpr(mCamera.GetHpr());
	mCamera.mMode = 1;
	cogAvailable = 0;
	mpGamepad = GetEngine()->FindComponent<CGamepadComponent>();

	//Setup enemies
	maxEnemies = 4;
	for (int x = -50; x < 50; x += 50)
	{
		for (int z = -50; z < 50; z += 50)
		{
			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain);
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(x, mpTerrain->GetHeight(x, z), z)));
			mpEnemy->mLife = 100;
			mEnemies.push_back(mpEnemy);
		}
	}

	//Setup boss
	mpBoss = new Boss(mpBossBulletMesh, mpTerrain);
	mpBoss->Init(mpBossMesh);
	mpBoss->SetPos(mpTerrain->GetPointOnGround(mpPlayer->GetPos() + D3DXVECTOR3(10, 0, 10)));
	mpBoss->mLife = 1000;
	bossActive = false;
	bossSpawnMinions = false;
	bossSpawned1 = false;
	bossSpawned2 = false;

	//Setup NPC
	mpNPC = new NPC();
	mpNPC->Init(mpNPCMesh);
	mpNPC->SetPos(mpTerrain->GetPointOnGround(mpPlayer->GetPos() + D3DXVECTOR3(10, 0, 0)));

	gameFont = CreateD3DFont(GetDevice(), "Segoe UI", 24, false);
}

void GameScene::Update(float dt)
{
	CScene::Update(dt);	// VITAL

	if (IsTopMost() == false) //Pause scene if not top most
		return;

	if (CGameWindow::KeyPress(VK_F10))
		ExitScene();

	if (CGameWindow::KeyPress(VK_F8))
		bossActive = !bossActive;

	if (CGameWindow::KeyPress(VK_F7))
		bossSpawnMinions = true;
	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_Y))
		{
			TalkToNpc();
		}
	}
	else
	{
		if (CGameWindow::KeyPress('Q'))
			TalkToNpc();
	}

	//Player
	mpPlayer->Update(dt);
	mpPlayer->UpdateWeapon(dt);
	if (mpPlayer->mPlayer.mLife <= 0) //show death scene when player is dead
	{
		ExitScene();
		GetEngine()->AddScene(new DeathScene());
	}

	//Update weather
	mpWeather->SetCentre(mCamera.GetPos());
	mpWeather->Update(dt);

	UpdateEnemies(mEnemies, &mpPlayer->mPlayer, dt);

	if (maxEnemies < 4)
	{
		int x = randi(-10, 10);
		int z = randi(-10, 10);

		mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain);
		mpEnemy->Init(mpEnemyMesh);
		mpEnemy->SetPos(mpTerrain->GetPointOnGround(D3DXVECTOR3(x, mpTerrain->GetHeight(x, z), z)));
		mpEnemy->mLife = 100;
		mEnemies.push_back(mpEnemy);

		maxEnemies += 1;
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

			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain);
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(left));
			mpEnemy->mLife = 200;
			mEnemies.push_back(mpEnemy);

			mpEnemy = new Enemy(mpEnemyBulletMesh, mpTerrain);
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(mpTerrain->GetPointOnGround(right));
			mpEnemy->mLife = 200;
			mEnemies.push_back(mpEnemy);
			bossSpawnMinions = false;
		}

		if (mpBoss->mLife <= 0) //Show victory scene when boss is killed
		{
			ExitScene();
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
	GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, BLACK_COL, 1.0f, 0);
	GetDevice()->BeginScene();

	// the terrain
	mpTerrain->Draw(IDENTITY_MAT, true);
	//Weather
	mpWeather->Draw(IDENTITY_MAT);

	//Enemies
	DrawEnemies(mEnemies);
	//Player
	mpPlayer->Draw();

	//Boss
	if (bossActive)
	{
		mpBoss->Draw();
		DrawMeshNodes(mpBoss->mBullets);
	}

	//NPC
	mpNPC->Draw();

	//Items
	DrawItems(mCog);

	//GetEngine()->DrawColourTint(D3DXCOLOR(0.4, 0, 0, (100 - mpPlayer->mPlayer.mLife) / 100));

	stringstream sout;
	sout << "Player pos: " << mpPlayer->mPlayer.GetPos();
	sout << "Player hpr: " << mpPlayer->mPlayer.GetHpr();
	sout << "\nNo of enemies: " << mEnemies.size();
	sout << "\nCurrent time: " << clock();
	sout << "\nClock: " << clock();
	sout << "\n DT: " << dt;
	sout << "\n FPS: " << GetEngine()->GetFps();
	sout << "\n Health: " << mpPlayer->mPlayer.mLife;
	sout << "\n boss Active: " << bossActive;
	sout << "\n Boss Health: " << mpBoss->mLife;
	sout << "\n Cogs Available: " << cogAvailable;

	DrawD3DFont(gameFont, sout.str().c_str(), 20, 20, RED_COL);

	GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	DrawSprite(GetSprite(), mpCrossHair, 0, 0);
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

		for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Bomb hitting enemy
		{
			if (CollisionMeshNode(mpPlayer->mRockets[b], mEnemies[en])) //Collision between bom and enemy
			{
				mpPlayer->mRockets[b]->Destroy();
				mEnemies[en]->Destroy();
				mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 3.0f), 50);
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

		if (mEnemies[en]->mLife == 0)
		{
			mpCogWheel = new Item();
			mpCogWheel->Init(mpCogWheelMesh);
			mpCogWheel->SetPos(mEnemies[en]->GetPos());
			mCog.push_back(mpCogWheel);
			maxEnemies -= 1;
		}
	}

	for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Bomb hitting ground
	{
		if ((mpPlayer->mRockets[b]->GetPos().y) < (mpTerrain->GetHeight(mpPlayer->mRockets[b]->GetPos().x, mpPlayer->mRockets[b]->GetPos().z))) //collision between bomb and ground
		{
			mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 3.0f), 50);
			mpPlayer->mRockets[b]->Destroy();
		}

		if (!mpPlayer->mRockets[b]->IsAlive())
		{
			mpPlayer->mRocketExplode->Explode(mpPlayer->mRockets[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(5.0f, 10.0f), 50);
		}
	}

	//Item
	for (int i = 0; i < mCog.size(); i++)
	{
		if (CollisionMeshNode(&mpPlayer->mPlayer, mCog[i]))
		{
			cogAvailable += mCog[i]->amount;
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

		for (int b = 0; b < mpPlayer->mRockets.size(); b++) //Player's missle/bomb
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