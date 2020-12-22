#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Player.h"
#include "World.h"
#include "Zombie.h"

namespace {
	//����min�`max�̃u���b�N�������v���C���[���痣�ꂽ���Ƀ]���r�𐶐�������B
	//���̂Ƃ���Axz�̃u���b�N�����ǂꂾ������Ă��邩�v�Z���Ă��܂��B
	//�R�[�h�̓s����Amax / 2 > min �ɂ��Ƃ��ĂˁB
	//24�`128���ƃ]���r�̐������킩��Â炢�̂Ŕ͈͂����߂Ă��܂��B
	//const int minDistanceZombieGenerate = 24;
	//const int maxDistanceZombieGenerate = 128;
	const int minDistanceZombieGenerate = 20;
	const int maxDistanceZombieGenerate = 50;
	//�ݒ肵���b����Ƀ]���r�̐������J�n����B
	const float zombieGenerateTimer = 7.0f;
	//��x�ɐ�������]���r�̍ő吔�B
	const int maxNumberZombieGenerateOneTime = 4;
	//�͈͓��ɐ�������]���r�̍ő吔�B
	//�͈͓��Ƃ����̂́A�v���C���[����̋���������max�̃u���b�N���ȓ��̂��́B
	const int maxNumberZombieRange = 10;
	//�S���[���h�ɐ�������]���r�̍ő吔�B
	const int maxNumberZombie = 30;
	//���������B
	std::mt19937 random((std::random_device())());
}

bool ZombieGenerator::Start()
{
	return true;
}

void ZombieGenerator::Update()
{
	if (GetPlayer() == nullptr)
	{
		return;
	}
	//�]���r�����B
	GenerateZombie();
	//�^�C�}�[���Z�B
	m_timer += GetEngine().GetRealDeltaTimeSec();
}

void ZombieGenerator::GenerateZombie()
{
	//��莞�Ԍo���Ă�����A�]���r�����B
	if (m_timer >= zombieGenerateTimer)
	{
		CVector3 playerPos = m_player->GetPos() / Block::WIDTH;
		m_numberZombieRange = 0;
		m_numberZombie = 0;
		//�͈͓��ɂ���]���r�̐��𒲂ׂ�B
		SearchNumberZombieRange(playerPos);
		//�]���r�𐶐��������񐔕��B
		for (int i = 0; i < maxNumberZombieGenerateOneTime; i++)
		{
			if (m_numberZombieRange >= maxNumberZombieRange || m_numberZombie >= maxNumberZombie)
			{
				break;
			}
			//����͈͓̔��̗����𐶐��A���ꂼ��x��z�B
			//-max / 2 �` -min / 2 �� min / 2 �` max / 2 �͈̗̔͂����B
			int ranX = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate / 2 + 1) + minDistanceZombieGenerate / 2;
			int ranZ = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate / 2 + 1) + minDistanceZombieGenerate / 2;
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
			//�]���r�������ł���X�y�[�X����������]���r�𐶐����Ă����B
			SearchSpaceZombieGenerate(pos);
		}
		//�^�C�}�[�����Z�b�g�B
		m_timer = 0.0f;
	}
}

void ZombieGenerator::SearchSpaceZombieGenerate(CVector3& pos)
{
	//������u���b�N�����݂��邩�ǂ��������Ă����B
	//�ォ�炾�Ɩ؂̏�Ƀ]���r�����������̂ŏォ��ɂ��܂����B
	for (int i = 0; i < Chunk::HEIGHT - 2; i++)
	{
		//�u���b�N�����݂��Ă�����B
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {
			//��2�}�X�ɋ󂫂����݂��Ă��邩�ǂ������ׂ�B
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				//�󂫂���������]���r�����B
				Zombie* zombie = m_world->CreateEntity<Zombie>();
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				m_numberZombieRange++;
				m_numberZombie++;
				return;
			}
		}
	}
}

void ZombieGenerator::SearchNumberZombieRange(CVector3& playerPos)
{
	//���[���h����G���e�B�e�B�̔z�񎝂��Ă���B
	std::unordered_set<Entity*> entities = m_world->GetEntities();
	for (auto entity : entities)
	{
		//�G���e�B�e�B�̃^�C�v���]���r��������B
		if (entity->GetEntityType() == enEntity_Zombie)
		{
			m_numberZombie++;
			CVector3 zombiePos = entity->GetPos() / Block::WIDTH;
			CVector3 diff = playerPos - zombiePos;
			//�v���C���[�Ƃ̋��������͈͓���������B
			if (diff.LengthSq() <= maxDistanceZombieGenerate * maxDistanceZombieGenerate)
			{
				//�͈͓��̃]���r�Ƃ��ăJ�E���g����B
				m_numberZombieRange++;
			}
		}
	}
}