#include "stdafx.h"
#include "MCCharaCon.h"
#include "World.h"
#include "Block.h"

namespace {
	const CVector3 BLOCK_SIZE = Block::WIDTH * 0.5f;
	constexpr float DOWN_OFFSET = 1.0f;
}

void MCCharaCon::Init(float width, float height, const CVector3& position) {
	m_colSize.x = m_colSize.z = width;
	m_colSize.y = height + width*2.0f;
	m_position = position;	
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

	//シフト移動(崖から落ちない)する?
	bool isShiftMoveMode = m_isOnGround && m_isShiftMove;

	//地上フラグ切る
	m_isOnGround = false;

	//i=0→XZ移動
	//i=1→Y移動
	std::vector<Block*> rtnBlocks;//接触するブロック
	for (int i = 0; i < 2; i++) {
		bool useDownOffset = false;

		//移動量を計算
		CVector3 moveVec = moveSpeed * deltaTime;
		if (m_isShiftMove) {
			//しゃがみ時の速度
			moveVec.x *= 0.3f;
			moveVec.z *= 0.3f;
		}
		if (i == 0) {
			//XZ方向の移動にする
			moveVec.y = 0.0f;
		}
		else {
			//Y方向の移動にする
			moveVec.x = moveVec.z = 0.0f;	
			if (abs(moveVec.y) < FLT_EPSILON) {
				//下方向へちょっと判定伸ばす
				moveVec.y -= DOWN_OFFSET;
				useDownOffset = true;
			}
		}
		float moveLength = moveVec.Length();

		//移動してない
		if (i == 0 && moveLength < FLT_EPSILON) {
			continue;
		}			
		//移動方向
		CVector3 moveDir = moveVec;
		moveDir.Normalize();

		CVector3 nowPos = m_position;//移動後位置
		float nowLength = 0.0f;//現在移動量
		bool isLast = false;//
		//最小移動量
		float minMove;
		if (i == 0) {
			minMove = min(Block::WIDTH*0.5f, m_colSize.x);
		}
		else {
			minMove = min(Block::WIDTH*0.5f, m_colSize.y);
		}

		int ColCnt = 0;
		while (ColCnt < 5) {
			rtnBlocks.clear();
			
			//シフト移動時は縁に仮想壁ブロックを追加
			constexpr int MAX_SIZE_VW = 16;
			std::unique_ptr<Block> virtualBlock[MAX_SIZE_VW];
			if (i == 0 && isShiftMoveMode) {
				//キャラAABB
				CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
				min.y = nowPos.y - Block::WIDTH / 2.0f - 1.0f;
				max.y = nowPos.y - Block::WIDTH / 2.0f;
				//足元のブロック取得
				if (GetBlocks(min, max, rtnBlocks)) {
					CVector3 createAABBmin, createAABBmax;
					bool first = true;
					for (auto block : rtnBlocks) {
						//ブロックAABB
						CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
						blockMin.y = block->GetModelPos().y;
						blockMax.y = block->GetModelPos().y + Block::WIDTH;
						//足元接触判定
						if (CMath::ColAABBs(min, max, blockMin, blockMax)) {
							if (first) {
								createAABBmin = blockMin;
								createAABBmax = blockMax;
								first = false;
							}
							else {
								createAABBmin.Min(blockMin);
								createAABBmax.Max(blockMax);
							}
						}
					}
					//仮想ブロックの作成
					if (!first) {
						int ci = 0;

						createAABBmax.x += Block::WIDTH;
						createAABBmin.x -= Block::WIDTH;
						for (int bi = 0; bi < (createAABBmax.x - createAABBmin.x) / Block::WIDTH; bi++) {
							CVector3 setPos;
							setPos.x = createAABBmin.x + (bi * Block::WIDTH) + (Block::WIDTH * 0.5f);
							setPos.y = createAABBmin.y + Block::WIDTH;
							setPos.z = createAABBmin.z - (Block::WIDTH * 0.5f) - (m_colSize.z * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							rtnBlocks.push_back(virtualBlock[ci].get());
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos()- BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							ci++;
							DW_ERRORBOX(ci == MAX_SIZE_VW, "シフト移動エラー")

							setPos.z = createAABBmax.z + (Block::WIDTH * 0.5f) + (m_colSize.z * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos() - BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							rtnBlocks.push_back(virtualBlock[ci].get());
							ci++;
							DW_ERRORBOX(ci > MAX_SIZE_VW, "シフト移動エラー")
						}
						createAABBmax.x += -Block::WIDTH;
						createAABBmin.x -= -Block::WIDTH;

						createAABBmax.z += Block::WIDTH;
						createAABBmin.z -= Block::WIDTH;
						for (int bi = 0; bi < (createAABBmax.z - createAABBmin.z) / Block::WIDTH; bi++) {
							CVector3 setPos;
							setPos.z = createAABBmin.z + (bi * Block::WIDTH) + (Block::WIDTH * 0.5f);
							setPos.y = createAABBmin.y + Block::WIDTH;
							setPos.x = createAABBmin.x - (Block::WIDTH * 0.5f) - (m_colSize.x * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos() - BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							rtnBlocks.push_back(virtualBlock[ci].get());
							ci++;
							DW_ERRORBOX(ci == MAX_SIZE_VW, "シフト移動エラー")

							setPos.x = createAABBmax.x + (Block::WIDTH * 0.5f) + (m_colSize.x * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos() - BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							rtnBlocks.push_back(virtualBlock[ci].get());
							ci++;
							DW_ERRORBOX(ci > MAX_SIZE_VW, "シフト移動エラー")
						}
					}
				}
			}

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
			if (!GetBlocks(min, max, rtnBlocks)) {
				return CVector3::Zero();
			}			

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

		if (useDownOffset && !m_isOnGround) {
			//余分に下移動した分戻す
			nowPos.y += DOWN_OFFSET;
		}

		m_position = nowPos;
	}

	//AABBレンダーの更新
	UpdateAABBRender();

	return m_position;
}

bool MCCharaCon::GetBlocks(const CVector3& aabbmin, const CVector3& aabbmax, std::vector<Block*>& return_blocks) {
	if (!m_world) {
		m_world = FindGO<World>(L"World");
		if (!m_world) {
			DW_ERRORBOX(true, "MCCharaCon::Execute() ワールドがありません");
			return false;
		}
	}
	m_world->GetBlocks(aabbmin, aabbmax, return_blocks);
	return true;
}
