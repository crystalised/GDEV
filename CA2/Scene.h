#pragma once
#include "CameraUtils.h"

static int cogAvailable;

//Gamepad
static CGamepadComponent* mpGamepad;
static bool leftClick;

class MenuScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	IDirect3DTexture9* mpButtonTex[8];
	IDirect3DTexture9* mpBGTex;
};

class GameScene : public CScene
{
	CameraUtils mCamera;
	CTerrain* mpTerrain;
	CPrecipitation* mpWeather;

	CXMesh* mpEnemyMesh;
	CXMesh* mpPlayerMesh;
	CXMesh* mpFlameMesh;
	CXMesh* mpRocketMesh;
	CXMesh* mpEnemyBulletMesh;
	CXMesh* mpBossMesh;
	CXMesh* mpBossBulletMesh;
	CXMesh* mpNPCMesh;
	CXMesh* mpCogWheelMesh;
	Player* mpPlayer;
	int maxEnemies;
	Enemy* mpEnemy;
	vector<Enemy*> mEnemies;
	Boss* mpBoss;
	bool bossActive;
	bool bossSpawnMinions;
	bool bossSpawned1, bossSpawned2;
	NPC* mpNPC;
	Item* mpCogWheel;
	vector<Item*> mCog;

	ID3DXFont* gameFont;

	IDirect3DTexture9* mpCrossHair; //Cross hair for aiming

public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	void CollisionCheck();
	void TalkToNpc();
};

class HelpScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};

class CreditScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};

class DeathScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
private:
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};

class VictoryScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
private:
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};

class NPCScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	IDirect3DTexture9* mpBackground;
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};