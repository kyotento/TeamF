#pragma once
#include "PairBlock.h"

class Bed : public PairBlock
{
public:
	Bed(bool isHeadParts, const IntVector3& pairOnDir);

	bool OnClick(Player* player) override;
};

