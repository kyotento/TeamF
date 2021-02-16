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
				m_sWorld->DestroyBlockNoDrop(pos);//原木なければ削除
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
	if (floor == 4) {//4が限界距離
		return false;
	}

	//原点とのマンハッタン距離
	int originDistance = abs(pos.x - originPos.x)+ abs(pos.y - originPos.y)+ abs(pos.z - originPos.z);

	//for (int i = 0; i < 6; i++) {
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {

				//原点回避
				if (x==0&& y == 0&& z == 0) {
					continue;
				}

				//サンプル位置算出
				IntVector3 nowPos = pos + IntVector3(x,y,z);//LightUtil::spreadDir[i]

				//原点に近づいてはならない
				int distance = abs(nowPos.x - originPos.x) + abs(nowPos.y - originPos.y) + abs(nowPos.z - originPos.z);
				if (floor > 0 && originDistance > distance) {
					continue;
				}

				auto block = m_sWorld->GetBlock(nowPos);
				if (block) {
					if (block->GetBlockType() == m_leafType) {//葉
						if (FindLog(nowPos, originPos, floor + 1)) {//検索続行
							return true;//原木あり
						}
					}
					if (block->GetBlockType() == m_woodType) {//原木
						return true;
					}
				}
				else {
					Chunk* chunk = m_sWorld->GetChunkFromWorldPos(nowPos.x, nowPos.z);
					if (!chunk || !chunk->IsCalcedSkyLight()) {
						//チャンクなしは原木ありとして扱う
						return true;
					}
				}

			}
		}
	}

	return false;//原木と接続されていない
}

/*
void WoodLog::DestoroyLeaf() {
	std::list<Block*> leaflist;
	if (!DestoroyLeafInner(CalcBlockUnitPos(), CalcBlockUnitPos(), 0, leaflist)) {
		//原木と接続されていないなら葉を破壊
		for (auto leaf : leaflist) {
			m_sWorld->DestroyBlockNoDrop(leaf->CalcBlockUnitPos());
		}
	}
}

bool WoodLog::DestoroyLeafInner(const IntVector3& pos, const IntVector3& originPos, int floor, std::list<Block*>& leaflist) {
	if (floor == 4) {
		return false;
	}

	for (int i = 0; i < 6; i++) {//6方向調べる(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == LightUtil::spreadDir[i].x && fromDir.y * -1 == LightUtil::spreadDir[i].y && fromDir.z * -1 == LightUtil::spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + LightUtil::spreadDir[i];

		auto block = m_sWorld->GetBlock(nowPos);
		if (block) {
			if (block->GetBlockType() == m_leafType) {//葉
				leaflist.emplace_back(block);
				if (DestoroyLeafInner(nowPos, LightUtil::spreadDir[i], floor + 1, leaflist)) {
					return true;
				}
			}
			if (block->GetBlockType() == m_woodType) {//原木
				return true;
			}
		}
	}

	return false;
}
*/
