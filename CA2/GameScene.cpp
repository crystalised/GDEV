#pragma once
#include "Scene.h"

#define M_PI 3.14159265358979323846

void GameScene::Enter()
{
	mpTerrain = new CTerrain(GetDevice(), "../media/Terrain/coastMountains.bmp", 5, 0.5);
	mpTerrain->GenerateTexture(D3DXVECTOR3(0.5f, 1, 0));
	// setup scene
	SetupDefaultD3DOptions(GetDevice(), true);
	// the models
	mpEnemyMesh = new CXMesh(GetDevice(), "../media/crate.x");
	mpPlayerMesh = new CXMesh(GetDevice(), "../media/yellow_ball.x");
	mpFlameMesh = new CXMesh(GetDevice(), "../media/yellow_ball.x");
	mpBombMesh = new CXMesh(GetDevice(), "../media/yellow_ball.x");

	//Setup player and camera
	mpPlayer = new Player(mpPlayerMesh, mpTerrain);
	mpPlayer->InitWeapon(GetDevice(), mpFlameMesh, mpBombMesh);
	mCamera.mMode = 1;

	for (int x = -50; x < 50; x += 10)
	{
		for (int z = -50; z < 50; z += 10)
		{
			mpEnemy = new Enemy(mpTerrain, GetDevice());
			mpEnemy->Init(mpEnemyMesh);
			mpEnemy->SetPos(D3DXVECTOR3(x, mpTerrain->GetHeight(x, z), z));
			mEnemies.push_back(mpEnemy);
		}
	}
}

void GameScene::Update(float dt)
{
	CScene::Update(dt);	// VITAL
	if (CGameWindow::KeyPress(VK_F10))
		ExitScene();

	mpPlayer->Update(dt);
	mpPlayer->UpdateWeapon(dt);

	UpdateEnemies(mEnemies, dt);

	//Collision check for weapon usage
	for (int en = 0; en < mEnemies.size(); en++)
	{
		for (int f = 0; f < mpPlayer->mFlames.size(); f++) //Flame thrower
		{
			if (CollisionMeshNode(mpPlayer->mFlames[f], mEnemies[en])) //Collision between flame and enemy
			{
				mpPlayer->mFlames[f]->Destroy();
				mEnemies[en]->Damage(1);
				mpPlayer->mFlameThrower->AddParticle(mEnemies[en]->GetPos(), mEnemies[en]->flameDirection);
			}
		}

		for (int b = 0; b < mpPlayer->mBombs.size(); b++) //Bomb
		{
			if (CollisionMeshNode(mpPlayer->mBombs[b], mEnemies[en])) //Collision between bom and enemy
			{
				mEnemies[en]->Destroy();
				mpPlayer->mBombExplode->Explode(mpPlayer->mBombs[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 2.5f));
				mpPlayer->mBombs[b]->Destroy();
			}
		}
	}

	for (int b = 0; b < mpPlayer->mBombs.size(); b++) //Bomb
	{
		if ((mpPlayer->mBombs[b]->GetPos().y) < (mpTerrain->GetHeight(mpPlayer->mBombs[b]->GetPos().x, mpPlayer->mBombs[b]->GetPos().z))) //collision between bomb and ground
		{
			mpPlayer->mBombExplode->Explode(mpPlayer->mBombs[b]->GetPos(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomColour(), CParticleSystem::GetRandomFloat(2.0f, 2.5f));
			mpPlayer->mBombs[b]->Destroy();
		}
	}
	DeleteDeadMeshNodes(mpPlayer->mFlames);
	DeleteDeadMeshNodes(mpPlayer->mBombs);
	DeleteDeadEnemies(mEnemies);

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

	DrawEnemies(mEnemies);
	//Draw player
	mpPlayer->Draw();

	GetDevice()->EndScene();
}

void GameScene::Leave()
{
	SAFE_DELETE(mpPlayer);
	SAFE_DELETE(mpTerrain);
	SAFE_DELETE(mpEnemyMesh);
	SAFE_DELETE(mpPlayerMesh);
	SAFE_DELETE(mpFlameMesh);
	SAFE_DELETE(mpBombMesh);

	//Delete all enemies
	for (int i = (int)mEnemies.size() - 1; i >= 0; i--)
	{
		delete mEnemies[i];
	}
	mEnemies.clear();
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	ENABLE_LEAK_DETECTION();	// Enable run-time memory check for debug builds.
	CSceneEngine eng(1280, 720, false);
	eng.AddScene(new GameScene);
	return eng.Go(hInstance);	// runs everything
}