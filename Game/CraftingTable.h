#pragma once
#include "Block.h"

class Player;

class CraftingTable :public Block{
public:
	bool OnClick( Player* player ) override;
};

