#include "stdafx.h"
#include "MobGenerator.h"
#include "Player.h"
#include "World.h"

namespace {
	//���������B
	std::mt19937 random((std::random_device())());
}

bool MobGenerator::Start()
{
	Init();
	return true;
}

void MobGenerator::Update()
{
	if (GetPlayer() == nullptr)
	{
		return;
	}
	//���u�����B
	GenerateMob();
	//�^�C�}�[���Z�B
	m_timer += GetEngine().GetRealDeltaTimeSec();
}

void MobGenerator::GenerateMob()
{
	//��莞�Ԍo���Ă�����A���u�����B
	if (m_timer >= m_params->mobGenerateTimer)
	{
		CVector3 playerPos = m_player->GetPos() / Block::WIDTH;
		m_numberMobRange = 0;
		m_numberMob = 0;
		//�͈͓��ɂ��郂�u�̐��𒲂ׂ�B
		SearchNumberMobRange(playerPos);
		//���u�𐶐��������񐔕��B
		for (int i = 0; i < m_params->maxNumberMobGenerateOneTime; i++)
		{
			if (m_numberMobRange >= m_params->maxNumberMobRange || m_numberMob >= m_params->maxNumberMob)
			{
				break;
			}
			//����͈͓̔��̗����𐶐��A���ꂼ��x��z�B
			//-max / 2 �` -min / 2 �� min / 2 �` max / 2 �͈̗̔͂����B
			int ranX = random() % (m_params->maxDistanceMobGenerate / 2 - m_params->minDistanceMobGenerate / 2 + 1) + m_params->minDistanceMobGenerate / 2;
			int ranZ = random() % (m_params->maxDistanceMobGenerate / 2 - m_params->minDistanceMobGenerate / 2 + 1) + m_params->minDistanceMobGenerate / 2;
			//�����ŗ]��0�������痐�����|�ɂ���B
			if (random() % 2)
			{
				ranX = -ranX;
			}
			if (random() % 2)
			{
				ranZ = -ranZ;
			}
			CVector3 pos = CVector3(playerPos.x + ranX, 0.0f, playerPos.z + ranZ);
			//���u�������ł���X�y�[�X���������烂�u�𐶐����Ă����B
			SearchSpaceMobGenerate(pos);
		}
		//�^�C�}�[�����Z�b�g�B
		m_timer = 0.0f;
	}
}

void MobGenerator::SearchSpaceMobGenerate(CVector3& pos)
{
	//������u���b�N�����݂��邩�ǂ��������Ă����B
	//�ォ�炾�Ɩ؂̏�Ƀ��u�����������̂ŏォ��ɂ��܂����B
	for (int i = 0; i < Chunk::HEIGHT - 2; i++)
	{
		//�u���b�N�����݂��Ă�����B
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {
			//��2�}�X�ɋ󂫂����݂��Ă��邩�ǂ������ׂ�B
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				//�󂫂��������烂�u�����B
				/*Zombie* zombie = NewGO<Zombie>(m_world);
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));*/
				NewGOMob(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				m_numberMobRange++;
				m_numberMob++;
				return;
			}
		}
	}
}

void MobGenerator::SearchNumberMobRange(CVector3& playerPos)
{
	//���[���h����G���e�B�e�B�̔z�񎝂��Ă���B
	std::unordered_set<Entity*> entities = m_world->GetEntities();
	for (auto entity : entities)
	{
		//�G���e�B�e�B�̃^�C�v�����u��������B
		if (entity->GetEntityType() == m_typeEntity)
		{
			m_numberMob++;
			CVector3 mobPos = entity->GetPos() / Block::WIDTH;
			CVector3 diff = playerPos - mobPos;
			//�v���C���[�Ƃ̋��������͈͓���������B
			if (diff.LengthSq() <= m_params->maxDistanceMobGenerate * m_params->maxDistanceMobGenerate)
			{
				//�͈͓��̃��u�Ƃ��ăJ�E���g����B
				m_numberMobRange++;
			}
		}
	}
}