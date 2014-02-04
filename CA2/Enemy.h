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

class Enemy : public CMeshNode
{
	// AI
	enum STATE {IDLING, CHASING, ATTACKING};
	static const int ATTACK_RANGE = 15; //Enemy attack range
	static const int DETECTION_RANGE = 30; //Enemy detection of player range
	const float SPEED = 3.0f; //Speed of enemy movement
	const float TURN = 2.0f; //Speed of enemy turn

public:
	CTerrain* mTerrain;
	//Enemy weapon
	CXMesh* mBulletMesh; //Model for Bullet
	vector<CMeshNode*> mBullets; //Vector for bullet node
	const int gun_CD = 2000;
	clock_t gun_used_time;

	const float groundOffset = 1.0f; //Offset for enemy from ground
	D3DXVECTOR3 flameDirection;

public:
	Enemy(CXMesh* inBulletMesh, CTerrain* inTerrain); //input enemy mesh and terrain to initialze player
	~Enemy();
	void Update(float dt, CMeshNode* inPlayer);
	void RotateTowardsTarget(float dirA, float dirB);
	void ChaseTarget(float inYaw, float inDelta);

private:
	STATE enemyState;
};

void UpdateEnemies(vector<Enemy*>& inEnemies, CMeshNode* inPlayer, float dt);
void DeleteDeadEnemies(vector<Enemy*>& inEnemies);
void DrawEnemies(vector<Enemy*>& inEnemies);