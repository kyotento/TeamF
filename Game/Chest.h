#pragma once
#include "Block.h"
#include "Inventory.h"

class Player;

class Chest :public Block {
public:
	Chest();
	~Chest();
	bool OnClick(Player* player) override;

private:
	//3�~9��27�̃X���b�g�B
	Inventory m_inventory;
};