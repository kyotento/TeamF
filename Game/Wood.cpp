#include "stdafx.h"
#include "Wood.h"
#include "World.h"
#include "BlockFactory.h"

void Leaf::Update() {
	if (++m_cnt >= REFLEASH_COUNT) {
		m_cnt = 0;
		IntVector3 pos = CalcBlockUnitPos();
		if (!FindLog(pos, pos, 0)) {
			//if (--m_leafHp <= 0) {
				m_sWorld->DestroyBlockNoDrop(pos);//���؂Ȃ���΍폜
				DW_WARNING_MESSAGE(true, "LEAF DELETE\n")
				//m_sWorld->SetBlock(pos.x, pos.y, pos.z, BlockFactory::CreateBlock(enCube_DiamondBlock));
			//}
			return;
		}
		//else {
			//m_leafHp = 6;
		//}
	}
}

bool Leaf::FindLog(const IntVector3& pos, const IntVector3& originPos, int floor) {
	if (floor == 4) {//4�����E����
		return false;
	}

	//���_�Ƃ̃}���n�b�^������
	int originDistance = abs(pos.x - originPos.x)+ abs(pos.y - originPos.y)+ abs(pos.z - originPos.z);

	//for (int i = 0; i < 6; i++) {
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {

				//���_���
				if (x==0&& y == 0&& z == 0) {
					continue;
				}

				//�T���v���ʒu�Z�o
				IntVector3 nowPos = pos + IntVector3(x,y,z);//LightUtil::spreadDir[i]

				//���_�ɋ߂Â��Ă͂Ȃ�Ȃ�
				int distance = abs(nowPos.x - originPos.x) + abs(nowPos.y - originPos.y) + abs(nowPos.z - originPos.z);
				if (floor > 0 && originDistance > distance) {
					continue;
				}

				auto block = m_sWorld->GetBlock(nowPos);
				if (block) {
					if (block->GetBlockType() == m_leafType) {//�t
						if (FindLog(nowPos, originPos, floor + 1)) {//�������s
							return true;//���؂���
						}
					}
					if (block->GetBlockType() == m_woodType) {//����
						return true;
					}
				}
				else {
					Chunk* chunk = m_sWorld->GetChunkFromWorldPos(nowPos.x, nowPos.z);
					if (!chunk || !chunk->IsCalcedSkyLight()) {
						//�`�����N�Ȃ��͌��؂���Ƃ��Ĉ���
						return true;
					}
				}

			}
		}
	}

	return false;//���؂Ɛڑ�����Ă��Ȃ�
}

/*
void WoodLog::DestoroyLeaf() {
	std::list<Block*> leaflist;
	if (!DestoroyLeafInner(CalcBlockUnitPos(), CalcBlockUnitPos(), 0, leaflist)) {
		//���؂Ɛڑ�����Ă��Ȃ��Ȃ�t��j��
		for (auto leaf : leaflist) {
			m_sWorld->DestroyBlockNoDrop(leaf->CalcBlockUnitPos());
		}
	}
}

bool WoodLog::DestoroyLeafInner(const IntVector3& pos, const IntVector3& originPos, int floor, std::list<Block*>& leaflist) {
	if (floor == 4) {
		return false;
	}

	for (int i = 0; i < 6; i++) {//6�������ׂ�(�O��㉺���E)
		//���������ɂ͖߂�Ȃ�
		if (fromDir.x * -1 == LightUtil::spreadDir[i].x && fromDir.y * -1 == LightUtil::spreadDir[i].y && fromDir.z * -1 == LightUtil::spreadDir[i].z) {
			continue;
		}

		//�T���v���ʒu�Z�o
		IntVector3 nowPos = pos + LightUtil::spreadDir[i];

		auto block = m_sWorld->GetBlock(nowPos);
		if (block) {
			if (block->GetBlockType() == m_leafType) {//�t
				leaflist.emplace_back(block);
				if (DestoroyLeafInner(nowPos, LightUtil::spreadDir[i], floor + 1, leaflist)) {
					return true;
				}
			}
			if (block->GetBlockType() == m_woodType) {//����
				return true;
			}
		}
	}

	return false;
}
*/
