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

	if (GetDistance(GetPos(), playerPos) < 2)
		inRange = true;
	else
		inRange = false;
}