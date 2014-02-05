#pragma once

#include "Node.h"
#include "Collision.h"

class NPC : public CMeshNode
{
public:
	bool inRange; //When player is in range and able to use NPC
	bool canFightBoss; //True when player completed the quest and accepted the challenge to fight the boss

public:
	NPC();
	~NPC();
	void Update(CMeshNode* inPlayer);
private:
};