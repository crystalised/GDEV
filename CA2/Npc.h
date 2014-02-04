#pragma once

#include "Node.h"
#include "Collision.h"

class NPC : public CMeshNode
{
public:
	bool inRange; //When player is in range and able to use NPC

public:
	NPC();
	~NPC();
	void Update(CMeshNode* inPlayer);
private:
};