#pragma once

#include "XMesh.h"
#include "Node.h"
#include "Terrain.h" //Terrain
#include "GameUtils.h"
#include "ParticleSystem.h"
#include "GameWindow.h"
#include "shot.h"
#include <vector>
#include <ctime>
#include "GamepadComponent.h"
#include "SceneEngine.h"

#define M_PI 3.14159265358979323846

using namespace std;

class Player : public CNode
{
public:
	CMeshNode mPlayer;
	CTerrain* mTerrain;
	//Weapons
	//Flame Thrower
	CParticleSystem* mFlameThrower;
	CXMesh* mFlameMesh; //Model for flames
	vector<CMeshNode*> mFlames; //Vector for flame node
	int flameThrowerLvl;
	//Bomb
	CParticleSystem* mBombTrail;
	CXMesh* mBombMesh;
	vector<CMeshNode*> mBombs;
	const int bomb_CD = 1000;
	clock_t bomb_used_time;
	CExplosion* mBombExplode;
	//Gamepad
	CGamepadComponent* mpGamepad;
	
	int currentWeapon;

	const float SPEED = 10.0f; //Speed of player movement
	const float turn = D2R(500); //Speed of player camera turn
	const float groundOffset = 1.0f; //Offset for player from ground

	//Player state for jumping motion
	enum STATE { JUMPING, FALLING, STANDING };
	STATE playerState;
	float jumpHeight; //Height the player jumps up to
	D3DXVECTOR3 gravity = D3DXVECTOR3(0, 9.81f, 0);

public:
	Player(CXMesh* inPlayerMesh, CTerrain* inTerrain, CSceneEngine* inEngine); //input player mesh and terrain to initialze player
	~Player();
	void Update(float dt);
	void Draw();
	void Init(IDirect3DDevice9* inDevice, CXMesh* inFlameMesh, CXMesh* inBombMesh);
	void ChangeWeapon(int inW);
	void UpdateWeapon(float dt);
	void Jump();
	void Shoot();
};