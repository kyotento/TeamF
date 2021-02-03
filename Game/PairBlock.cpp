#include "stdafx.h"
#include "PairBlock.h"
#include "World.h"

PairBlock::~PairBlock() {
	if (m_pairType != enCube_None) {
		//�w������ɗאڂ���w��^�C�v�̃u���b�N��j�󂷂�
		IntVector3 pos = CalcBlockUnitPos() + m_pairOnDir;
		auto block = m_sWorld->GetBlock(pos);
		if (block && block->GetBlockType() == m_pairType) {
			m_sWorld->DestroyBlockNoDrop(pos);
		}
	}
}