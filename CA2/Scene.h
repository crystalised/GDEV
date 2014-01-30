#pragma once
#include "CameraUtils.h"

class MenuScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};

class GameScene : public CScene
{
	CameraUtils mCamera;
	CTerrain* mpTerrain;

	CXMesh* mpEnemyMesh;
	CXMesh* mpPlayerMesh;
	CXMesh* mpFlameMesh;
	CXMesh* mpBombMesh;
	Player* mpPlayer;
	Enemy* mpEnemy;
	vector<Enemy*> mEnemies;

public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};

class HelpScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};

class CreditScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};

class LoseScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};

class WinScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
};