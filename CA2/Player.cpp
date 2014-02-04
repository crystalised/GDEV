#include "Player.h"

Player::Player(CXMesh* inPlayerMesh, CTerrain* inTerrain, CSceneEngine* inEngine) //Input player mesh and terrain, init player
{
	mTerrain = inTerrain;
	mPlayer.Init(inPlayerMesh);
	mPlayer.SetPos(mTerrain->GetPointOnGround(mPlayer.GetPos(), groundOffset));
	mPlayer.mLife = 100;
	playerState = STANDING; //Player default state is standing
	currentWeapon = 1; //Player default weapon is flame thrower(1)
	bomb_used_time = 0;

	//Init gamepad
	mpGamepad = inEngine->FindComponent<CGamepadComponent>();
}

Player::~Player()
{
	SAFE_DELETE(mFlameThrower);
	DeleteMeshNodes(mFlames);
	DeleteMeshNodes(mBombs);
	SAFE_DELETE(mBombExplode);
	SAFE_DELETE(mBombTrail);
}

void Player::Update(float dt) //Update player based on input
{
	D3DXVECTOR3 move; //Player move vector
	D3DXVECTOR3 turnCam = D3DXVECTOR3(0.f, 0.f, 0.f); //Turn cam vector
	if (mpGamepad->IsGamepadConnected(0))
	{
		move = mpGamepad->GetVector(0, LEFT_X, NONE, LEFT_Y);
		turnCam = mpGamepad->GetVector(0, RIGHT_Y_INV, RIGHT_X, NONE) / 5; //reduce sensitivity
		if (mpGamepad->IsButtonDown(0, XINPUT_GAMEPAD_A))
			Jump();
	}
	else
	{
		move = GetKeyboardVector(WSAD_KEYS);
		turnCam = GetMouseTurnVector(100, true);

		if (CGameWindow::KeyPress(VK_SPACE))
		{
			Jump();
		}

		if (CGameWindow::KeyPress('1'))
		{
			ChangeWeapon(1);
		}
		else if (CGameWindow::KeyPress('2'))
		{
			ChangeWeapon(2);
		}
	}

	mPlayer.Move(move * dt * SPEED);
	mPlayer.Turn(turnCam * dt * turn);

	//Limit player from looking past straight up or down
	float limit = (float)(M_PI / 2) - 0.01f;

	mPlayer.mHpr.x = (float)__max(-limit, mPlayer.mHpr.x);
	mPlayer.mHpr.x = (float)__min(+limit, mPlayer.mHpr.x);

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
}

void Player::Draw()
{
	mPlayer.Draw();
	mFlameThrower->Draw(IDENTITY_MAT);
	DrawMeshNodes(mBombs);
	mBombExplode->Draw(IDENTITY_MAT);
	//DrawMeshNodes(mFlames);
	mBombTrail->Draw(IDENTITY_MAT);
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

	mBombMesh = inBombMesh;
	mBombExplode = new CExplosion();
	mBombExplode->Init(inDevice, "../media/Particle/spark.bmp");

	settings.LifeTime = 0.15f;
	settings.StartColor = D3DCOLOR_XRGB(255, 141, 29);
	settings.Size = 0.2;

	mBombTrail = new CParticleSystem();
	mBombTrail->Init(inDevice, "../media/Particle/flare.bmp", settings);
}

void Player::ChangeWeapon(int inW)
{
	CGameWindow::ClearKeyPress();

	currentWeapon = inW; //Set current weapon to input
}

void Player::UpdateWeapon(float dt)
{
	if (mpGamepad->IsGamepadConnected(0))
	{
		if (mpGamepad->GetRightTrigger(0) > 0)
		{
			Shoot();
		}
		if (mpGamepad->IsButtonPressed(0, XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			if (currentWeapon == 1)
				currentWeapon = 2;
			else if (currentWeapon == 2)
				currentWeapon = 1;
		}
	}
	else
	{
		if (CGameWindow::KeyDown(VK_LBUTTON))
		{
			Shoot();
		}
	}

	for (int i = 0; i < mBombs.size(); i++)
	{
		mBombTrail->AddParticle(mBombs[i]->GetPos(), D3DXVECTOR3(0, 0, 0));
	}

	mFlameThrower->Update(dt);
	mBombExplode->Update(dt);
	mBombTrail->Update(dt);
	UpdateMeshNodes(mFlames, dt, mTerrain);
	UpdateMeshNodes(mBombs, dt);
}

void Player::Jump()
{
	CGameWindow::ClearKeyPress();
	if (playerState == STANDING)
	{
		jumpHeight = mPlayer.GetPos().y + 3;
		playerState = JUMPING;
	}
}

void Player::Shoot()
{
	CGameWindow::ClearKeyPress();
	const D3DXVECTOR3 FORWARD(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 0.2f, 1.0f); //Direction of particles
	D3DXVECTOR3 vel = mPlayer.RotateVector(FORWARD);	// gets camera's forward

	switch (currentWeapon)
	{
	case 1:
	{
		vel *= CParticleSystem::GetRandomFloat(0.2f, 1.0f) * 200;

		CShot* shot = new CShot();
		// this is the direction the player is facing
		// setup the shot starting at player location & direction
		shot->Init(mFlameMesh, mPlayer.GetPos(), mPlayer.GetHpr());
		shot->mPos = mPlayer.GetPos() + D3DXVECTOR3(-0.2f, -0.2f, 0);
		shot->mVel = vel;
		shot->mScale = 0.5f;	// small
		shot->mLife = 50; //Short life
		mFlames.push_back(shot);	// add to vector

		mFlameThrower->AddParticle(mPlayer.GetPos() + D3DXVECTOR3(-0.2f, -0.2f, 0), vel); //Spawnpoint of particles
		break;
	}
	case 2:
		if (clock() - bomb_used_time > bomb_CD)
		{
			vel *= 25;

			CShot* shot = new CShot();
			shot->Init(mBombMesh, mPlayer.GetPos(), mPlayer.GetHpr());
			shot->mPos = mPlayer.GetPos() + D3DXVECTOR3(-0.2f, 0, -0.5f);
			shot->mVel = vel;
			shot->mScale = 1.0f;
			shot->mLife = 2000;
			mBombs.push_back(shot);
			bomb_used_time = clock();
			break;
		}
	}
}