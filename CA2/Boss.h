#pragma once

#include "XMesh.h"
#include "Node.h"
#include "Terrain.h" //Terrain
#include "GameUtils.h"
#include "ParticleSystem.h"
#include "GameWindow.h"
#include "Collision.h"
#include "shot.h"
#include <ctime>

#define M_PI 3.14159265358979323846

using namespace std;

class Boss : public CMeshNode
{
	// AI
	enum STATE { IDLING, CHASING, ATTACKING };
	static const int ATTACK_RANGE = 25; //Boss attack range
	static const int DETECTION_RANGE = 100; //Boss detection of player range
	static const int SPEED = 6; //Speed of boss movement
	static const int TURN = 3; //Speed of boss turn

public:
	CTerrain* mTerrain;
	//Boss weapon
	CXMesh* mBulletMesh; //Model for Bullet
	vector<CMeshNode*> mBullets; //Vector for bullet node
	int gun_CD;
	clock_t gun_used_time;
	CParticleSystem* bossPowerUp;

	static const int groundOffset = 0; //Offset for boss from ground
	D3DXVECTOR3 flameDirection;

public:
	Boss(CXMesh* inBulletMesh, CTerrain* inTerrain, IDirect3DDevice9* inDevice); //input boss mesh and terrain to initialze player
	~Boss();
	void Update(float dt, CMeshNode* inPlayer);
	void RotateTowardsTarget(float dirA, float dirB);
	void ChaseTarget(float inYaw, float inDelta);

private:
	STATE bossState;
};