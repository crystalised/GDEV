#pragma once
#include "CameraUtils.h"

static int cogAvailable;

//Graphics
static ID3DXFont* mpFont;
static IDirect3DTexture9* mpButtonB;

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
	CXMesh* mpHutMesh;

	Player* mpPlayer;
	int maxEnemies;
	Enemy* mpEnemy;
	vector<Enemy*> mEnemies;
	Boss* mpBoss;
	bool bossActive;
	bool bossSpawnMinions;
	bool bossSpawned1, bossSpawned2;
	NPC* mpNPC;
	NPC* mpHut; //non npc, using for placeholder
	Item* mpCogWheel;
	vector<Item*> mCog;

	bool inTown;
	const int tele_CD = 15000;
	clock_t tele_used_time;
	bool teleportToBoss;

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
	void HandleInput(float dt);
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
	IDirect3DTexture9* mpBG1;
	IDirect3DTexture9* mpBG2;
};

class CreditScene : public CScene
{
public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();

private:
	IDirect3DTexture9* mpBG;
	IDirect3DTexture9* mpButtonTex[2];
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
	IDirect3DTexture9* mpBG;
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
	IDirect3DTexture9* mpBG;
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
	IDirect3DTexture9* mpButtonA;
};