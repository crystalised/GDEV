#pragma once

#include "XMesh.h"
#include "Node.h"
#include "Terrain.h" //Terrain
#include "GameUtils.h"
#include "ParticleSystem.h"
#include "GameWindow.h"

#define M_PI 3.14159265358979323846

using namespace std;

class Enemy : public CMeshNode
{
public:
	CTerrain* mTerrain;

	const float SPEED = 10.0f; //Speed of player movement
	const float turn = D2R(500); //Speed of player camera turn
	const float groundOffset = 1.0f; //Offset for player from ground
	D3DXVECTOR3 flameDirection;

public:
	Enemy(CTerrain* inTerrain, IDirect3DDevice9* inDevice); //input player mesh and terrain to initialze player
	~Enemy();
	void Update(float dt);
};

void UpdateEnemies(vector<Enemy*>& inEnemies, float dt);
void DeleteDeadEnemies(vector<Enemy*>& inEnemies);
void DrawEnemies(vector<Enemy*>& inEnemies);