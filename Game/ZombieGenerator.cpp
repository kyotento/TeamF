#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Player.h"
#include "World.h"
#include "Zombie.h"

namespace {
	//����min�`max�̃u���b�N�������v���C���[���痣�ꂽ���Ƀ]���r�𐶐�������B
	//���̂Ƃ���Axz�̃u���b�N�����ǂꂾ������Ă��邩�v�Z���Ă��܂��B
	//�R�[�h�̓s����Amax / 2 > min �ɂ��Ƃ��ĂˁB
	const int minDistanceZombieGenerate = 24;
	const int maxDistanceZombieGenerate = 128;
	//�ݒ肵���b����Ƀ]���r�̐������J�n����B
	const float zombieGenerateTimer = 10.0f;
	//��x�ɐ�������]���r�̍ő吔�B
	const int maxNumberZombieGenerate = 3;
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
		//�]���r�𐶐��������񐔕��B
		for (int i = 0; i < maxNumberZombieGenerate; i++)
		{
			//����͈͓̔��̗����𐶐��A���ꂼ��x��y�B
			int ranX = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate + 1) + minDistanceZombieGenerate;
			int ranZ = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate + 1) + minDistanceZombieGenerate;
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
			SearchSpaceZombieGenerate(pos);
		}
		//�^�C�}�[�����Z�b�g�B
		m_timer = 0.0f;
	}
}

void ZombieGenerator::SearchSpaceZombieGenerate(CVector3& pos)
{
	//�ォ��u���b�N�����݂��邩�ǂ��������Ă����B
	for (int i = Chunk::HEIGHT; i > 0; i--)
	{
		//�u���b�N�����݂��Ă�����B
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {

			//��2�}�X�ɋ󂫂����݂��Ă��邩�ǂ������ׂ�B
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				Enemy* zombie = NewGO<Zombie>(m_world);
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				break;
			}
		}


	}
}