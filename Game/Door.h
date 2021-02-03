#pragma once
#include "PairBlock.h"

class Door : public PairBlock
{
public:
	Door(bool isThisUp) : PairBlock(isThisUp ? enCube_DoorDown : enCube_DoorUp, isThisUp ? IntVector3(0, -1, 0) : IntVector3(0, 1, 0)) {}

	bool OnClick(Player* player) override;
};

