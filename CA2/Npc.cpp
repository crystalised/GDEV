#include "Npc.h"

NPC::NPC()
{

}

NPC::~NPC()
{

}

void NPC::Update(CMeshNode* inPlayer)
{
	D3DXVECTOR3 playerPos = inPlayer->GetPos();

	LookAt(D3DXVECTOR3(playerPos.x, 0, playerPos.z));

	if (GetDistance(GetPos(), playerPos) < 2)
		inRange = true;
	else
		inRange = false;
}