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
	//Rocket
	CParticleSystem* mRocketTrail;
	CXMesh* mRocketMesh;
	vector<CMeshNode*> mRockets;
	static const int rocket_CD = 5000;
	clock_t rocket_used_time;
	CExplosion* mRocketExplode;
	bool rocketReady;
	
	int currentWeapon;

	int SPEED; //Speed of player movement
	float turn; //Speed of player camera turn
	float groundOffset; //Offset for player from ground

	//Player state for jumping motion
	enum STATE { JUMPING, FALLING, STANDING };
	STATE playerState;
	float jumpHeight; //Height the player jumps up to
	D3DXVECTOR3 gravity;

public:
	Player(CXMesh* inPlayerMesh, CTerrain* inTerrain); //input player mesh and terrain to initialze player
	~Player();
	void Update(float dt);
	void Draw();
	void Init(IDirect3DDevice9* inDevice, CXMesh* inFlameMesh, CXMesh* inRocketMesh);
	void ChangeWeapon(int inW);
	void UpdateWeapon(float dt);
	void Jump();
	void Shoot();
};