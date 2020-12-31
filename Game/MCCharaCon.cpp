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
	CVector3 moveDir = moveVec; //moveDir.y = 0.0f;
	moveDir.Normalize();

	CVector3 nowPos = m_position;
	float nowLength = 0.0f;
	bool isLast = false;
	std::vector<Block*> rtnBlocks;
	const float minMove = min(min(Block::WIDTH, m_colSize.x), m_colSize.y);
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

		//キャラクターのAABBに接触するブロック達を取得
		CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
		min.y = nowPos.y;
		max.y = nowPos.y + m_colSize.y;		
		rtnBlocks.clear();
		m_world->GetBlocks( min, max, rtnBlocks);

		if (!rtnBlocks.empty()) {
			CVector3 hitNoraml;
			float pushf[6] = {};

			//プレイヤーAABB
			CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
			min.y = nowPos.y;
			max.y = nowPos.y + m_colSize.y;
			for (auto block : rtnBlocks) {
				//ブロックAABB
				CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
				blockMin.y = block->GetModelPos().y;
				blockMax.y = block->GetModelPos().y + Block::WIDTH;

				bool isHit[6] = {};
				CVector3 planeMin, planeMax;
				//ブロックAABBとX-面の判定
				planeMin = min, planeMax = max; planeMax.x = planeMin.x;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[0] = true;
					pushf[0] = max(pushf[0], blockMax.x - planeMin.x);
				}
				//ブロックAABBとX+面の判定
				planeMin = min, planeMax = max; planeMin.x = planeMax.x;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[1] = true;
					pushf[1] = min(pushf[1], blockMin.x - planeMax.x);
				}
				//ブロックAABBとY-面の判定
				planeMin = min, planeMax = max; planeMax.y = planeMin.y;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[2] = true;
					pushf[2] = max(pushf[2], blockMax.y - planeMin.y);
				}
				//ブロックAABBとY+面の判定
				planeMin = min, planeMax = max; planeMin.y = planeMax.y;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[3] = true;
					pushf[3] = min(pushf[3], blockMin.y - planeMax.y);
				}
				//ブロックAABBとZ-面の判定
				planeMin = min, planeMax = max; planeMax.z = planeMin.z;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[4] = true;
					pushf[4] = max(pushf[4], blockMax.z - planeMin.z);
				}
				//ブロックAABBとZ+面の判定
				planeMin = min, planeMax = max; planeMin.z = planeMax.z;
				if (CMath::ColAABBs(planeMin, planeMax, blockMin, blockMax)) {
					isHit[5] = true;
					pushf[5] = min(pushf[5], blockMin.z - planeMax.z);
				}
				//衝突法線の算出
				if (isHit[0] && !isHit[1]) {
					hitNoraml.x += 1.0f;
				}
				if (!isHit[0] && isHit[1]) {
					hitNoraml.x += -1.0f;
				}
				/*if (isHit[2] && !isHit[3]) {
					hitNoraml.y = 1.0f;
				}
				if (!isHit[2] && isHit[3]) {
					hitNoraml.y = -1.0f;
				}*/
				if (isHit[4] && !isHit[5]) {
					hitNoraml.z += 1.0f;
				}
				if (!isHit[4] && isHit[5]) {
					hitNoraml.z += -1.0f;
				}

				hitNoraml.Normalize();

				nowPos += hitNoraml * hitNoraml.Dot(moveVec) * -1.0f;

				hitNoraml = CVector3::Zero();

			}
			//hitNoraml.Normalize();

			//nowPos += hitNoraml * hitNoraml.Dot(moveVec) * -1.0f;


			//if (hitNoraml.x > FLT_EPSILON) {
			//	nowPos.x += pushf[0];
			//}
			//if (hitNoraml.x < FLT_EPSILON) {
			//	nowPos.x += pushf[1];
			//}
			/////*if (hitNoraml.y > FLT_EPSILON) {
			//	nowPos.y += pushf[2];
			//}
			//if (hitNoraml.y < FLT_EPSILON) {
			//	nowPos.y += pushf[3];
			//}//*/
			//if (hitNoraml.z > FLT_EPSILON) {
			//	nowPos.z += pushf[4];
			//}
			//if (hitNoraml.z < FLT_EPSILON) {
			//	nowPos.z += pushf[5];
			//}

			//抜けるまでループ
			/*float backLenrth = 0.0f;
			while (backLenrth < Block::WIDTH*2.0f) {	
				bool isbreak = true;
				for (auto block : rtnBlocks) {
					//プレイヤーAABB
					CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
					min.y = nowPos.y;
					max.y = nowPos.y + m_colSize.y;
					//ブロックAABB
					CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
					blockMin.y = block->GetModelPos().y;
					blockMax.y = block->GetModelPos().y + Block::WIDTH;
					//衝突判定
					if (CMath::ColAABBs(min, max, blockMin, blockMax)) {
						backLenrth += 1.0f;
						nowPos += hitNoraml * 1.0f;
						isbreak = false;
						break;
					}
				}
				if (isbreak) {
					break;
				}
			}*/
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
