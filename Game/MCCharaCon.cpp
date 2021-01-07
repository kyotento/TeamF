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
	//�n��t���O�؂�
	m_isOnGround = false;

	//i=0��XZ�ړ�
	//i=1��Y�ړ�
	for (int i = 0; i < 2; i++) {
		//�ړ��ʂ��v�Z
		CVector3 moveVec = moveSpeed * deltaTime;
		if (i == 0) {
			//XZ�����̈ړ��ɂ���
			moveVec.y = 0.0f;
		}
		else {
			//Y�����̈ړ��ɂ���
			moveVec.x = moveVec.z = 0.0f;
		}
		float moveLength = moveVec.Length();

		//�ړ����ĂȂ�
		if (moveLength < FLT_EPSILON) {
			continue;
		}		

		//�ړ�����
		CVector3 moveDir = moveVec;
		moveDir.Normalize();

		CVector3 nowPos = m_position;//�ړ���ʒu
		float nowLength = 0.0f;//���݈ړ���
		bool isLast = false;//
		std::vector<Block*> rtnBlocks;//�ڐG����u���b�N
		//�ŏ��ړ���
		float minMove;
		if (i == 0) {
			minMove = min(Block::WIDTH*0.5f, m_colSize.x);
		}
		else {
			minMove = min(Block::WIDTH*0.5f, m_colSize.y);
		}

		int ColCnt = 0;
		while (moveLength > FLT_EPSILON && ColCnt < 5) {
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
			rtnBlocks.clear();
			m_world->GetBlocks(min, max, rtnBlocks);

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
		m_position = nowPos;
	}

	//AABB�����_�[�̍X�V
	UpdateAABBRender();

	return m_position;
}
