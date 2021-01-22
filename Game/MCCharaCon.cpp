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
		DW_WARNING_MESSAGE(true,"���̃L�����R���͏���������ĂȂ���")
		return CVector3::Zero();
	}	

	//�W�����v���ɂ���
	if (moveSpeed.y > 0.0f) {
		m_isJump = true;
		m_isOnGround = false;
	}
	//�ǐڐG�t���O�؂�
	m_isContactWall = false;

	//�V�t�g�ړ�(�R���痎���Ȃ�)����?
	bool isShiftMoveMode = m_isOnGround && m_isShiftMove;

	//�n��t���O�؂�
	m_isOnGround = false;

	//i=0��XZ�ړ�
	//i=1��Y�ړ�
	std::vector<Block*> rtnBlocks;//�ڐG����u���b�N
	for (int i = 0; i < 2; i++) {
		bool useDownOffset = false;

		//�ړ��ʂ��v�Z
		CVector3 moveVec = moveSpeed * deltaTime;
		if (m_isShiftMove) {
			//���Ⴊ�ݎ��̑��x
			moveVec.x *= 0.3f;
			moveVec.z *= 0.3f;
		}
		if (i == 0) {
			//XZ�����̈ړ��ɂ���
			moveVec.y = 0.0f;
		}
		else {
			//Y�����̈ړ��ɂ���
			moveVec.x = moveVec.z = 0.0f;	
			if (abs(moveVec.y) < FLT_EPSILON) {
				//�������ւ�����Ɣ���L�΂�
				moveVec.y -= DOWN_OFFSET;
				useDownOffset = true;
			}
		}
		float moveLength = moveVec.Length();

		//�ړ����ĂȂ�
		if (i == 0 && moveLength < FLT_EPSILON) {
			continue;
		}			
		//�ړ�����
		CVector3 moveDir = moveVec;
		moveDir.Normalize();

		CVector3 nowPos = m_position;//�ړ���ʒu
		float nowLength = 0.0f;//���݈ړ���
		bool isLast = false;//
		//�ŏ��ړ���
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
			
			//�V�t�g�ړ����͉��ɉ��z�ǃu���b�N��ǉ�
			constexpr int MAX_SIZE_VW = 16;
			std::unique_ptr<Block> virtualBlock[MAX_SIZE_VW];
			if (i == 0 && isShiftMoveMode) {
				//�L����AABB
				CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
				min.y = nowPos.y - Block::WIDTH / 2.0f - 1.0f;
				max.y = nowPos.y - Block::WIDTH / 2.0f;
				//�����̃u���b�N�擾
				if (GetBlocks(min, max, rtnBlocks)) {
					CVector3 createAABBmin, createAABBmax;
					bool first = true;
					for (auto block : rtnBlocks) {
						//�u���b�NAABB
						CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
						blockMin.y = block->GetModelPos().y;
						blockMax.y = block->GetModelPos().y + Block::WIDTH;
						//�����ڐG����
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
					//���z�u���b�N�̍쐬
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
							DW_ERRORBOX(ci == MAX_SIZE_VW, "�V�t�g�ړ��G���[")

							setPos.z = createAABBmax.z + (Block::WIDTH * 0.5f) + (m_colSize.z * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos() - BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							rtnBlocks.push_back(virtualBlock[ci].get());
							ci++;
							DW_ERRORBOX(ci > MAX_SIZE_VW, "�V�t�g�ړ��G���[")
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
							DW_ERRORBOX(ci == MAX_SIZE_VW, "�V�t�g�ړ��G���[")

							setPos.x = createAABBmax.x + (Block::WIDTH * 0.5f) + (m_colSize.x * 2.0f - m_offset);

							virtualBlock[ci] = std::make_unique<Block>();
							virtualBlock[ci]->SetPosWithWorldPos(setPos);
							//m_aabbReender2[ci].Init(virtualBlock[ci]->GetModelPos() - BLOCK_SIZE, virtualBlock[ci]->GetModelPos() + BLOCK_SIZE, { 0.0f,0.0f,1.0f,1.0f });
							rtnBlocks.push_back(virtualBlock[ci].get());
							ci++;
							DW_ERRORBOX(ci > MAX_SIZE_VW, "�V�t�g�ړ��G���[")
						}
					}
				}
			}

			//�������ړ������Ĕ���
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

			//�L����AABB
			CVector3 min = nowPos - m_colSize, max = nowPos + m_colSize;
			min.y = nowPos.y; max.y = nowPos.y + m_colSize.y;

			//�L�����N�^�[��AABB�ɐڐG����u���b�N�B���擾
			if (!GetBlocks(min, max, rtnBlocks)) {
				return CVector3::Zero();
			}			

			bool isHit = false;
			if (!rtnBlocks.empty()) {
				float minimumDistance = -1.0f;

				while (true) {
					//�L�����Ɉ�ԋ߂��u���b�N�����߂�
					float distance = -1.0f;
					CVector3 NearBlockMin, NearBlockMax;
					for (auto block : rtnBlocks) {
						//�u���b�NAABB
						CVector3 blockMin = block->GetModelPos() - BLOCK_SIZE, blockMax = block->GetModelPos() + BLOCK_SIZE;
						blockMin.y = block->GetModelPos().y;
						blockMax.y = block->GetModelPos().y + Block::WIDTH;

						//�ł����S���m�̋������߂��u���b�N�Ɣ���
						CVector3 pushD = (min + max) * 0.5f - (blockMin + blockMax) * 0.5f;
						if (minimumDistance < pushD.LengthSq() && (distance < 0.0f || distance > pushD.LengthSq())) {
							distance = pushD.LengthSq();
							NearBlockMin = blockMin, NearBlockMax = blockMax;
						}
					}
					minimumDistance = distance;

					//���肷����̂Ȃ�
					if (distance < 0.0f) {
						break;
					}

					//�����o��
					for (int pushCnt = 0; pushCnt < Block::WIDTH * 2; pushCnt++) {
						//�L����AABB�X�V
						min = nowPos - m_colSize, max = nowPos + m_colSize;
						min.y = nowPos.y; max.y = nowPos.y + m_colSize.y;

						//�Փ˔���
						if (CMath::ColAABBs(min, max, NearBlockMin, NearBlockMax)) {
							//�����o��
							CVector3 pushD = (min + max) * 0.5f - (NearBlockMin + NearBlockMax) * 0.5f;
							if (i == 0) {
								//X or Z�����̈ړ��ɂ���
								pushD.y = 0.0f;
								if (abs(pushD.x) > abs(pushD.z)) {
									pushD.z = 0.0f;
									//�����o����
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
									//�����o����
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
								//�ǂɂ�������
								m_isContactWall = true;
							}
							else {
								//Y�����̈ړ��ɂ���
								pushD.x = pushD.z = 0.0f;
								if (pushD.y < 0.0f) {
									//�V��ɂ�������
									if (moveSpeed.y > 0.0f) {
										moveSpeed.y *= -1.0f;
									}
									//�����o����
									pushD.y = -m_offset;
									if (pushCnt == 0) {
										pushD.y += NearBlockMin.y - max.y;
									}
								}
								else {
									//���ɂ�������
									m_isJump = false;
									m_isOnGround = true;
									moveSpeed.y = 0.0f;
									//�����o����
									pushD.y = m_offset;
									if (pushCnt == 0) {
										pushD.y += NearBlockMax.y - min.y;
									}
								}
							}
							//�����o��
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
			//�]���ɉ��ړ��������߂�
			nowPos.y += DOWN_OFFSET;
		}

		m_position = nowPos;
	}

	//AABB�����_�[�̍X�V
	UpdateAABBRender();

	return m_position;
}

bool MCCharaCon::GetBlocks(const CVector3& aabbmin, const CVector3& aabbmax, std::vector<Block*>& return_blocks) {
	if (!m_world) {
		m_world = FindGO<World>(L"World");
		if (!m_world) {
			DW_ERRORBOX(true, "MCCharaCon::Execute() ���[���h������܂���");
			return false;
		}
	}
	m_world->GetBlocks(aabbmin, aabbmax, return_blocks);
	return true;
}