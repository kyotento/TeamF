#pragma once
#include "Block.h"

class PairBlock : public Block
{
public:
	PairBlock(EnCube pairType, const IntVector3& pairOnDir) : m_pairType(pairType), m_pairOnDir(pairOnDir){}
	virtual ~PairBlock();

protected:
	EnCube m_pairType = enCube_None;//ペアブロックの種類
	IntVector3 m_pairOnDir;//ペアブロックがある方向
};

