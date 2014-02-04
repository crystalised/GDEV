#pragma once
#include "CameraUtils.h"

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
	CXMesh* mpBombMesh;
	CXMesh* mpEnemyBulletMesh;
	CXMesh* mpBossMesh;
	CXMesh* mpBossBulletMesh;
	CXMesh* mpNPCMesh;
	Player* mpPlayer;
	Enemy* mpEnemy;
	vector<Enemy*> mEnemies;
	Boss* mpBoss;
	bool bossActive;
	bool bossSpawnMinions;
	NPC* mpNPC;

	ID3DXFont* gameFont;

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
	IDirect3DTexture9* mpButtonTex[2];
	ID3DXFont* mpFont;
};