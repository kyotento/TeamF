#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"
#include "Creeper.h"
#include "Sun.h"

namespace {
	//���v�B
	float sunsetSecond = 64800.0f;
	//���̏o�B
	float sunriseSecond = 21600.0f;
}

void ZombieGenerator::Init()
{
	//�����̗l�X�ȃp�����[�^��ݒ肷��B
	GeneratorParameters* params = new GeneratorParameters();
	SetParametors(params);

	//��������G���e�B�e�B�̃^�C�v��ݒ肷��B
	SetTypeEntity(enEntity_Zombie);
}

void ZombieGenerator::NewGOMob(const CVector3& pos) 
{
	if (m_sun == nullptr)
	{
		m_sun = FindGO<Sun>();
		return;
	}
	//Zombie* zombie = NewGO<Zombie>();
	float second = m_sun->GetSecond();
	//���v��茻�������i��ł���A���邢�͓��̏o��莞�����O��������B
	//�]���r�𐶐�����B
	if (second > sunsetSecond || second < sunriseSecond) {
		//�����n�_�̖��邳�����ȉ�
		char light = *MobGenerator::GetWorld()->GetLightData((int)std::floorf(pos.x / Block::WIDTH), (int)std::floorf(pos.y / Block::WIDTH), (int)std::floorf(pos.z / Block::WIDTH));
		if (light <= 4) {
			//�v���C���[�̐����B
			Enemy* zombie = nullptr;
			if (CMath::RandomZeroToOne() < 0.1f) {
				zombie = MobGenerator::GetWorld()->CreateEntity<Creeper>();
			}
			else {
				zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
			}
			zombie->SetPos(pos);
		}
	}
}