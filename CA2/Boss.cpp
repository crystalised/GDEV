#include "Boss.h"

Boss::Boss(CXMesh* inBulletMesh, CTerrain* inTerrain)
{
	mTerrain = inTerrain;
	flameDirection = D3DXVECTOR3(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 1.0f, 0.2f) * (CParticleSystem::GetRandomFloat(0.2f, 1.0f) * 10);
	bossState = IDLING;

	//Init boss weapon
	mBulletMesh = inBulletMesh;
}

Boss::~Boss()
{
	DeleteMeshNodes(mBullets);
}

void Boss::Update(float dt, CMeshNode* inPlayer)
{
		D3DXVECTOR3 bossPos = GetPos();
		D3DXVECTOR3 playerPos = inPlayer->GetPos();
		float yaw = GetHpr().y;
		float delta = GetDirection(playerPos - bossPos);
		NormalizeRotation(this);
		switch (bossState)
		{
		case IDLING:
			if (GetDistance(inPlayer->GetPos(), bossPos) <= DETECTION_RANGE)
			{
				bossState = CHASING;
			}
			break;
		case CHASING:
			if (GetDistance(inPlayer->GetPos(), bossPos) > DETECTION_RANGE)
			{
				bossState = IDLING;
			}
			if (GetDistance(inPlayer->GetPos(), bossPos) <= ATTACK_RANGE)
			{
				bossState = ATTACKING;
			}
			ChaseTarget(yaw, delta);
			break;
		case ATTACKING:
			if (GetDistance(inPlayer->GetPos(), bossPos) > DETECTION_RANGE)
			{
				bossState = IDLING;
			}
			if (GetDistance(inPlayer->GetPos(), bossPos) > ATTACK_RANGE)
			{
				bossState = CHASING;
			}
			RotateTowardsTarget(yaw, delta);

			D3DXVECTOR3 FORWARD(0.f, 0.f, 1);
			D3DXVECTOR3 vel = RotateVector(FORWARD);
			if (clock() - gun_used_time > gun_CD)
			{
				vel *= 25;
				CShot* shot = new CShot();
				shot->Init(mBulletMesh, GetPos(), GetHpr());
				shot->mPos = GetPos();
				shot->mVel = vel;
				shot->mScale = 0.5f;
				shot->mLife = 3000;
				mBullets.push_back(shot);
				if (mLife <= 900)
				{
					FORWARD = D3DXVECTOR3(CParticleSystem::GetRandomFloat(0.2, 0.5), 0, 1);
					D3DXVECTOR3 vel2 = RotateVector(FORWARD);
					vel2 *= 25;
					CShot* shot2 = new CShot();
					shot2->Init(mBulletMesh, GetPos(), GetHpr());
					shot2->mPos = GetPos();
					shot2->mVel = vel2;
					shot2->mScale = 0.5f;
					shot2->mLife = 3000;
					mBullets.push_back(shot2);

					FORWARD = D3DXVECTOR3(-CParticleSystem::GetRandomFloat(0.2, 0.5), 0, 1);
					D3DXVECTOR3 vel3 = RotateVector(FORWARD);
					vel3 *= 25;
					CShot* shot3 = new CShot();
					shot3->Init(mBulletMesh, GetPos(), GetHpr());
					shot3->mPos = GetPos();
					shot3->mVel = vel3;
					shot3->mScale = 0.5f;
					shot3->mLife = 3000;
					mBullets.push_back(shot3);
				}
				gun_used_time = clock();
				break;
			}
			break;
		}
		UpdateMeshNodes(mBullets, dt, mTerrain);
		DeleteDeadMeshNodes(mBullets);
}

void Boss::RotateTowardsTarget(float dirA, float dirB)
{
	if (NormalizeAngle(dirA - dirB) > 0) //If angle between enemy and player is positive
		Yaw(D2R(-1 * TURN)); //Rotate backwards
	else if (NormalizeAngle(dirA - dirB) < 0) //Else if its negative
		Yaw(D2R(1 * TURN)); //Rotate forward
}

void Boss::ChaseTarget(float inYaw, float inDelta)
{
	if (!(GetDeltaDirection(inYaw, inDelta) < D2R(TURN * 1.5)))
	{
		RotateTowardsTarget(inYaw, inDelta);
	}
	Move(D3DXVECTOR3(0, 0, D2R(1 * SPEED)));
	SetPos(mTerrain->GetPointOnGround(GetPos(), groundOffset));
}