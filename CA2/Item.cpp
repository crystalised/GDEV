#include "Item.h"

Item::Item()
{
	amount = randi(1, 3); //Set a random amount that drops
}

Item::~Item()
{

}

void Item::Update(float dt)
{
	Yaw(D2R(dt * 60));
}

void DeleteCollectedItems(vector<Item*>& inItems)
{
	for (int i = (int)inItems.size() - 1; i >= 0; i--)
	{
		if (inItems[i]->IsAlive() == false)
		{
			delete inItems[i];
			inItems.erase(inItems.begin() + i);
		}
	}
}

void DrawItems(vector<Item*>& inItems)
{
	for (int i = (int)inItems.size() - 1; i >= 0; i--)
	{
		if (inItems[i]->IsAlive())
		{
			inItems[i]->Draw();
		}
	}
}