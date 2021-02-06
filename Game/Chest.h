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
	//3×9の27個のスロット。
	Inventory m_inventory;
};