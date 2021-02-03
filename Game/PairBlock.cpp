#include "stdafx.h"
#include "PairBlock.h"
#include "World.h"

PairBlock::~PairBlock() {
	if (m_pairType != enCube_None) {
		//指定方向に隣接する指定タイプのブロックを破壊する
		IntVector3 pos = CalcBlockUnitPos() + m_pairOnDir;
		auto block = m_sWorld->GetBlock(pos);
		if (block && block->GetBlockType() == m_pairType) {
			m_sWorld->DestroyBlockNoDrop(pos);
		}
	}
}