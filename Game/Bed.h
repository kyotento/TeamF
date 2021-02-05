#pragma once
#include "PairBlock.h"

class Bed : public PairBlock
{
public:
	Bed(bool isHeadParts, enMuki muki);

	bool OnClick(Player* player) override;
};

