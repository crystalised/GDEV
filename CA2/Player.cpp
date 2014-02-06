#include "Player.h"

Player::Player(CXMesh* inPlayerMesh, CTerrain* inTerrain) //Input player mesh and terrain, init player
{
	mTerrain = inTerrain;
	mPlayer.Init(inPlayerMesh);
	mPlayer.SetPos(D3DXVECTOR3(53.93f, mTerrain->GetHeight(53.93f, 481.04f), 481.04f));
	mPlayer.mLife = 100;
	playerState = STANDING; //Player default state is standing
	currentWeapon = 1; //Player default weapon is flame thrower(1)
	rocket_used_time = 0;
}

Player::~Player()
{
	SAFE_DELETE(mFlameThrower);
	DeleteMeshNodes(mFlames);
	DeleteMeshNodes(mRockets);
	SAFE_DELETE(mRocketExplode);
	SAFE_DELETE(mRocketTrail);
}

void Player::Update(float dt) //Update player based on input
{
	if (mPlayer.GetPos().y < 25.0f)
	{
		mPlayer.mLife = 0;
	}

	switch (playerState)
	{
		case JUMPING:
			mPlayer.SetPos(mPlayer.GetPos() + gravity * dt);
			if (mPlayer.GetPos().y > jumpHeight)
				playerState = FALLING;
			break;
		case FALLING:
			mPlayer.SetPos(mPlayer.GetPos() - gravity * dt);
			if (mPlayer.GetPos().y <= mTerrain->GetPointOnGround(mPlayer.GetPos(), groundOffset).y)
				playerState = STANDING;
			break;
		case STANDING:
			mPlayer.SetPos(mTerrain->GetPointOnGround(mPlayer.GetPos(), groundOffset));
			break;
	}

	if (clock() - rocket_used_time > rocket_CD)
		rocketReady = true;
	else
		rocketReady = false;

}

void Player::Draw()
{
	mPlayer.Draw();
	mFlameThrower->Draw(IDENTITY_MAT);
	DrawMeshNodes(mRockets);
	mRocketExplode->Draw(IDENTITY_MAT);
	//DrawMeshNodes(mFlames);
	mRocketTrail->Draw(IDENTITY_MAT);
}

void Player::Init(IDirect3DDevice9* inDevice, CXMesh* inFlameMesh, CXMesh* inBombMesh)
{
	mFlameMesh = inFlameMesh;

	flameThrowerLvl = 1;

	SParticleSetting settings;
	settings.Size = 0.25f;
	settings.LifeTime = 0.5f;
	settings.StartColor = D3DCOLOR_XRGB(0, 42, 255);	// start bright
	settings.EndColor = BLACK_COL;	// cool to black
	// additive blend
	settings.SourceBlend = D3DBLEND_ONE;
	settings.DestBlend = D3DBLEND_ONE;
	settings.MaxParticles = 500;
	mFlameThrower = new CParticleSystem();
	mFlameThrower->Init(inDevice, "../media/Particle/flare.bmp", settings);

	mRocketMesh = inBombMesh;
	mRocketExplode = new CExplosion();
	mRocketExplode->Init(inDevice, "../media/Particle/spark.bmp");

	settings.LifeTime = 0.5f;
	settings.StartColor = D3DCOLOR_XRGB(255, 141, 29);
	settings.Size = 0.15;

	mRocketTrail = new CParticleSystem();
	mRocketTrail->Init(inDevice, "../media/Particle/flare.bmp", settings);
}

void Player::ChangeWeapon(int inW)
{
	CGameWindow::ClearKeyPress();

	currentWeapon = inW; //Set current weapon to input
}

void Player::UpdateWeapon(float dt)
{
	for (int i = 0; i < mRockets.size(); i++)
	{
		D3DXVECTOR3 vel = mPlayer.RotateVector(D3DXVECTOR3(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 0.2f, 1.0f));
		vel *= -50;
		mRocketTrail->AddParticle(mRockets[i]->GetPos(), vel);
	}

	mFlameThrower->Update(dt);
	mRocketExplode->Update(dt);
	mRocketTrail->Update(dt);
	UpdateMeshNodes(mFlames, dt, mTerrain);
	UpdateMeshNodes(mRockets, dt);
}

void Player::Jump()
{
	CGameWindow::ClearKeyPress();
	if (playerState == STANDING)
	{
		jumpHeight = mPlayer.GetPos().y + 4;
		playerState = JUMPING;
	}
}

void Player::Shoot()
{
	CGameWindow::ClearKeyPress();
	D3DXVECTOR3 vel;
	
	switch (currentWeapon)
	{
	case 1:
	{
		D3DXVECTOR3 FORWARD(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 0, 1.0f); //Direction of particles
		vel = mPlayer.RotateVector(FORWARD);	// gets camera's forward
		vel *= CParticleSystem::GetRandomFloat(0.2f, 1.0f) * 200;

		CShot* shot = new CShot();
		// this is the direction the player is facing
		// setup the shot starting at player location & direction
		shot->Init(mFlameMesh, mPlayer.GetPos(), mPlayer.GetHpr());
		shot->mPos = mPlayer.GetPos() + D3DXVECTOR3(0, 0.3f, 0);
		shot->mVel = vel;
		shot->mScale = 0.5f;	// small
		shot->mLife = 50; //Short life
		mFlames.push_back(shot);	// add to vector

		mFlameThrower->AddParticle(mPlayer.GetPos() + D3DXVECTOR3(0, 0.3f, 0), vel); //Spawnpoint of particles
		break;
	}
	case 2:
		if (clock() - rocket_used_time > rocket_CD)
		{
			D3DXVECTOR3 FORWARD(0, 0, 1.0f); //Direction of particles
			vel = mPlayer.RotateVector(FORWARD);
			vel *= 25;

			CShot* shot = new CShot();
			shot->Init(mRocketMesh, mPlayer.GetPos(), mPlayer.GetHpr());
			shot->mPos = mPlayer.GetPos() + D3DXVECTOR3(-0.2f, 0.5f, -0.5f);
			shot->mVel = vel;
			shot->mScale = 1.0f;
			shot->mLife = 2000;
			mRockets.push_back(shot);
			rocket_used_time = clock();
			break;
		}
	}
}