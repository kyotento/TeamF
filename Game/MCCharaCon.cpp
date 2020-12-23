#include "stdafx.h"
#include "MCCharaCon.h"
#include "World.h"
#include "Block.h"

namespace {
	const CVector3 BLOCK_SIZE = Block::WIDTH * 0.5f;
}

void MCCharaCon::Init(float width, float height, const CVector3& position, World* world) {
	m_colSize.x = m_colSize.z = width;
	m_colSize.y = height + width*2.0f;
	m_position = position;
	m_world = world;
	m_isInited = true;
}

const CVector3& MCCharaCon::Execute(CVector3& moveSpeed, float deltaTime) {
	if (!m_isInited) {
		DW_WARNING_MESSAGE(true,"このキャラコンは初期化されてないよ")
		return CVector3::Zero();
	}

	if (moveSpeed.y > 0.0f) {
		//吹っ飛び中にする。
		m_isJump = true;
		m_isOnGround = false;
	}

	//移動量を計算
	CVector3 moveVec = moveSpeed * deltaTime;
	float moveLength = moveVec.Length();
	if (moveLength < FLT_EPSILON) {
		//移動してない
		return m_position;
	}
	CVector3 moveDir = moveVec;
	moveDir.Normalize();

	CVector3 nowPos = m_position;
	float nowLength = 0.0f;
	bool isLast = false;
	std::vector<Block*> rtnBlocks;
	while(1){
		//一ブロックずつ移動させて判定
		if (nowLength + Block::WIDTH >= moveLength) {
			isLast = true;
			nowPos += moveDir * (moveLength - nowLength);
			nowLength += (moveLength - nowLength);
		}
		else {
			nowPos += moveDir * Block::WIDTH;
			nowLength += Block::WIDTH;
		}

		CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
		min.y = nowPos.y;
		max.y = nowPos.y + m_colSize.y;		
		rtnBlocks.clear();
		m_world->GetBlocks( min, max, rtnBlocks);

		if (!rtnBlocks.empty()) {
			for (auto block : rtnBlocks) {
				float backLenrth = 0.0f;
				while (backLenrth < Block::WIDTH) {
					CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
					min.y = nowPos.y;
					max.y = nowPos.y + m_colSize.y;

					CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
					blockMin.y = block->GetModelPos().y;
					blockMax.y = block->GetModelPos().y + Block::WIDTH;

					if (CMath::ColAABBs(min, max, blockMin, blockMax)) {
						backLenrth += 1.0f;
						nowPos -= moveDir * 1.0f;
					}
					else {
						break;
					}
				}
			}
			isLast = true;
		}

		if (isLast) {
			break;
		}
	}
	m_position = nowPos;

	//AABBレンダーの更新
	UpdateAABBRender();

	return m_position;
}
