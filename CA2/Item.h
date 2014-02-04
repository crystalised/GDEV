#pragma once

#include "Node.h"
#include "Collision.h"
#include "GameUtils.h"

using namespace std;

class Item : public CMeshNode
{
public:
	int amount;
public:
	Item();
	~Item();
	void Update(float dt);
};

void DeleteCollectedItems(vector<Item*>& inItems);
void DrawItems(vector<Item*>& inItems);