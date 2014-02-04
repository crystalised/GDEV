#include "Enemy.h"

Enemy::Enemy(CXMesh* inBulletMesh, CTerrain* inTerrain) //Input enemy mesh and terrain, init player
{
	mTerrain = inTerrain;
	flameDirection = D3DXVECTOR3(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 1.0f, 0.2f) * (CParticleSystem::GetRandomFloat(0.2f, 1.0f) * 100);
	enemyState = IDLING;

	//Init enemy weapon
	mBulletMesh = inBulletMesh;
}


Enemy::~Enemy()
{
	DeleteMeshNodes(mBullets);
}

void Enemy::Update(float dt, CMeshNode* inPlayer)
{
	D3DXVECTOR3 enemyPos = GetPos(); //Enemy pos
	D3DXVECTOR3 playerPos = inPlayer->GetPos(); //Player pos
	float yaw = GetHpr().y; //Yaw of enemy node
	float delta = GetDirection(playerPos - enemyPos); //Direction of player from enemy
	NormalizeRotation(this);
	switch (enemyState)
	{
	case IDLING:
		if (GetDistance(inPlayer->GetPos(), enemyPos) <= DETECTION_RANGE)
		{
			enemyState = CHASING;
		}
		break;
	case CHASING:
		if (GetDistance(inPlayer->GetPos(), enemyPos) > DETECTION_RANGE) //Out of range
		{
			enemyState = IDLING;
		}
		if (GetDistance(inPlayer->GetPos(), enemyPos) <= ATTACK_RANGE) //Within attack range
		{
			enemyState = ATTACKING;
		}
		ChaseTarget(yaw, delta);
		break;
	case ATTACKING:
		if (GetDistance(inPlayer->GetPos(), enemyPos) > DETECTION_RANGE) //Out of range
		{
			enemyState = IDLING;
		}
		if (GetDistance(inPlayer->GetPos(), enemyPos) > ATTACK_RANGE)//Out of attack range
		{
			enemyState = CHASING;
		}
		RotateTowardsTarget(yaw, delta);

		const D3DXVECTOR3 FORWARD(0.f, 0.f, 1);
		D3DXVECTOR3 vel = RotateVector(FORWARD);
		if (clock() - gun_used_time > gun_CD)
		{
			vel *= 5;
			CShot* shot = new CShot();
			shot->Init(mBulletMesh, GetPos(), GetHpr());
			shot->mPos = mTerrain->GetPointOnGround(GetPos());
			shot->mVel = vel;
			shot->mScale = 0.5f;
			shot->mLife = 3000;
			mBullets.push_back(shot);
			gun_used_time = clock();
			break;
		}
		break;
	}
	UpdateMeshNodes(mBullets, dt, mTerrain);
	DeleteDeadMeshNodes(mBullets);
}

void Enemy::RotateTowardsTarget(float dirA, float dirB)
{
	if (NormalizeAngle(dirA - dirB) > 0) //If angle between enemy and player is positive
		Yaw(D2R(-1 * TURN)); //Rotate backwards
	else if (NormalizeAngle(dirA - dirB) < 0) //Else if its negative
		Yaw(D2R(1 * TURN)); //Rotate forward
}

void Enemy::ChaseTarget(float inYaw, float inDelta)
{
	if (!(GetDeltaDirection(inYaw, inDelta) < D2R(TURN * 1.5)))
	{
		RotateTowardsTarget(inYaw, inDelta);
	}
	Move(D3DXVECTOR3(0, 0, D2R(1 * SPEED)));
	SetPos(mTerrain->GetPointOnGround(GetPos(), groundOffset));
}

void UpdateEnemies(vector<Enemy*>& inEnemies, CMeshNode* inPlayer, float dt)
{
	for (int i = (int)inEnemies.size() - 1; i >= 0; i--)
	{
		if (inEnemies[i]->IsAlive() == true)
		{
			inEnemies[i]->Update(dt, inPlayer);
		}
	}
}

void DeleteDeadEnemies(vector<Enemy*>& inEnemies)
{
	for (int i = (int)inEnemies.size() - 1; i >= 0; i--)
	{
		if (inEnemies[i]->IsAlive() == false)
		{
			delete inEnemies[i];
			inEnemies.erase(inEnemies.begin() + i);
		}
	}
}

void DrawEnemies(vector<Enemy*>& inEnemies)
{
	for (int i = (int)inEnemies.size() - 1; i >= 0; i--)
	{
		if (inEnemies[i]->IsAlive())
		{
			inEnemies[i]->Draw();
			DrawMeshNodes(inEnemies[i]->mBullets);
		}
	}
}