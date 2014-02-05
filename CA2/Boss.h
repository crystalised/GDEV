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
	static const int ATTACK_RANGE = 15; //Boss attack range
	static const int DETECTION_RANGE = 20; //Boss detection of player range
	const float SPEED = 6.0f; //Speed of boss movement
	const float TURN = 3.0f; //Speed of boss turn

public:
	CTerrain* mTerrain;
	//Boss weapon
	CXMesh* mBulletMesh; //Model for Bullet
	vector<CMeshNode*> mBullets; //Vector for bullet node
	const int gun_CD = 1000;
	clock_t gun_used_time;

	const float groundOffset = 0; //Offset for boss from ground
	D3DXVECTOR3 flameDirection;

public:
	Boss(CXMesh* inBulletMesh, CTerrain* inTerrain); //input boss mesh and terrain to initialze player
	~Boss();
	void Update(float dt, CMeshNode* inPlayer);
	void RotateTowardsTarget(float dirA, float dirB);
	void ChaseTarget(float inYaw, float inDelta);

private:
	STATE bossState;
};