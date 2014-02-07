#pragma once
#include "CameraUtils.h"

static int cogObtained;
static bool inQuest; //Whether player is in quest
static bool questComplete; //Whether quest is completed
static bool teleportToBoss;

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
	D3DXVECTOR3 lastPos;
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
	static const int tele_CD = 15000;
	clock_t tele_used_time;
	bool teleportToBoss;

	bool teleAvailable;

	ID3DXFont* gameFont;

	IDirect3DTexture9* mpCrossHair; //Cross hair for aiming
	IDirect3DTexture9* mpUI; //User Interface
	IDirect3DTexture9* mpMissileIcon; //Icon for Missile
	IDirect3DTexture9* mpFireIcon; //icon for Flamethrower

public:
	void Enter();
	void Update(float dt);
	void Draw(float dt);
	void Leave();
	static int GetCogObtained();
	static bool GetQuestStatus();

private:
	IDirect3DTexture9* mpCogGUI;
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
	static bool GetQuestStatus();
	static bool TeleToBoss();

private:
	POINT mousepos;
	IDirect3DTexture9* mpBackground[5];
	IDirect3DTexture9* mpButtonTex[10];
	IDirect3DTexture9* mpButtonA;
	int questStage;
};