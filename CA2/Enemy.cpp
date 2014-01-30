#include "Enemy.h"

Enemy::Enemy(CTerrain* inTerrain, IDirect3DDevice9* inDevice) //Input player mesh and terrain, init player
{
	mTerrain = inTerrain;
	flameDirection = D3DXVECTOR3(CParticleSystem::GetRandomFloat(-0.1f, 0.1f), 1.0f, 0.2f) * (CParticleSystem::GetRandomFloat(0.2f, 1.0f) * 100);
}


Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
}

void UpdateEnemies(vector<Enemy*>& inEnemies, float dt)
{
	for (int i = (int)inEnemies.size() - 1; i >= 0; i--)
	{
		if (inEnemies[i]->IsAlive() == true)
		{
			inEnemies[i]->Update(dt);
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
		if (inEnemies[i]->IsAlive() == true)
		{
			inEnemies[i]->Draw();
		}
	}
}