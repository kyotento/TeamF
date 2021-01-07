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

	//ジャンプ中にする
	if (moveSpeed.y > 0.0f) {
		m_isJump = true;
		m_isOnGround = false;
	}
	//壁接触フラグ切る
	m_isContactWall = false;
	//地上フラグ切る
	m_isOnGround = false;

	//i=0→XZ移動
	//i=1→Y移動
	for (int i = 0; i < 2; i++) {
		//移動量を計算
		CVector3 moveVec = moveSpeed * deltaTime;
		if (i == 0) {
			//XZ方向の移動にする
			moveVec.y = 0.0f;
		}
		else {
			//Y方向の移動にする
			moveVec.x = moveVec.z = 0.0f;
		}
		float moveLength = moveVec.Length();

		//移動してない
		if (moveLength < FLT_EPSILON) {
			continue;
		}		

		//移動方向
		CVector3 moveDir = moveVec;
		moveDir.Normalize();

		CVector3 nowPos = m_position;//移動後位置
		float nowLength = 0.0f;//現在移動量
		bool isLast = false;//
		std::vector<Block*> rtnBlocks;//接触するブロック
		//最小移動量
		float minMove;
		if (i == 0) {
			minMove = min(Block::WIDTH*0.5f, m_colSize.x);
		}
		else {
			minMove = min(Block::WIDTH*0.5f, m_colSize.y);
		}

		int ColCnt = 0;
		while (moveLength > FLT_EPSILON && ColCnt < 5) {
			//少しずつ移動させて判定
			if (!isLast) {
				if (nowLength + minMove >= moveLength) {
					isLast = true;
					nowPos += moveDir * (moveLength - nowLength);
					nowLength += (moveLength - nowLength);
				}
				else {
					nowPos += moveDir * minMove;
					nowLength += minMove;
				}
			}
			else {
				ColCnt++;
			}

			//キャラAABB
			CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
			min.y = nowPos.y; max.y = nowPos.y + m_colSize.y;

			//キャラクターのAABBに接触するブロック達を取得
			rtnBlocks.clear();
			m_world->GetBlocks(min, max, rtnBlocks);

			bool isHit = false;
			if (!rtnBlocks.empty()) {
				float minimumDistance = -1.0f;

				while (true) {
					//キャラに一番近いブロックを求める
					float distance = -1.0f;
					CVector3 NearBlockMin, NearBlockMax;
					for (auto block : rtnBlocks) {
						//ブロックAABB
						CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
						blockMin.y = block->GetModelPos().y;
						blockMax.y = block->GetModelPos().y + Block::WIDTH;

						//最も中心同士の距離が近いブロックと判定
						CVector3 pushD = (min + max) * 0.5f - (blockMin + blockMax) * 0.5f;
						if (minimumDistance < pushD.LengthSq() && (distance < 0.0f || distance > pushD.LengthSq())) {
							distance = pushD.LengthSq();
							NearBlockMin = blockMin, NearBlockMax = blockMax;
						}
					}
					minimumDistance = distance;

					//判定するものなし
					if (distance < 0.0f) {
						break;
					}

					//押し出し
					for (int pushCnt = 0; pushCnt < Block::WIDTH * 2; pushCnt++) {
						//キャラAABB更新
						min = nowPos - m_colSize, max = nowPos + m_colSize;
						min.y = nowPos.y; max.y = nowPos.y + m_colSize.y;

						//衝突判定
						if (CMath::ColAABBs(min, max, NearBlockMin, NearBlockMax)) {
							//押し出し
							CVector3 pushD = (min + max) * 0.5f - (NearBlockMin + NearBlockMax) * 0.5f;
							if (i == 0) {
								//X or Z方向の移動にする
								pushD.y = 0.0f;
								if (abs(pushD.x) > abs(pushD.z)) {
									pushD.z = 0.0f;
									//押し出し量
									pushD.x = pushD.x < 0.0f ? -m_offset : m_offset;
									if (pushCnt == 0) {
										if (pushD.x < 0.0f) {
											pushD.x += NearBlockMin.x - max.x;
										}
										else {
											pushD.x += NearBlockMax.x - min.x;
										}
									}
								}
								else {
									pushD.x = 0.0f;
									//押し出し量
									pushD.z = pushD.z < 0.0f ? -m_offset : m_offset;
									if (pushCnt == 0) {
										if (pushD.z < 0.0f) {
											pushD.z += NearBlockMin.z - max.z;
										}
										else {
											pushD.z += NearBlockMax.z - min.z;
										}
									}
								}
								//壁にあたった
								m_isContactWall = true;
							}
							else {
								//Y方向の移動にする
								pushD.x = pushD.z = 0.0f;
								if (pushD.y < 0.0f) {
									//天井にあたった
									if (moveSpeed.y > 0.0f) {
										moveSpeed.y *= -1.0f;
									}
									//押し出し量
									pushD.y = -m_offset;
									if (pushCnt == 0) {
										pushD.y += NearBlockMin.y - max.y;
									}
								}
								else {
									//床にあたった
									m_isJump = false;
									m_isOnGround = true;
									moveSpeed.y = 0.0f;
									//押し出し量
									pushD.y = m_offset;
									if (pushCnt == 0) {
										pushD.y += NearBlockMax.y - min.y;
									}
								}
							}
							//押し出す
							nowPos += pushD;
							isHit = true;
							isLast = true;
						}
						else {
							break;
						}
					}
				}
			}
			if (isLast && !isHit) {
				break;
			}
		}
		m_position = nowPos;
	}

	//AABBレンダーの更新
	UpdateAABBRender();

	return m_position;
}
