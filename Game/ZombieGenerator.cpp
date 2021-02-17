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

	float second = m_sun->GetSecond();
	IntVector3 sampPos = { (int)std::floorf(pos.x / Block::WIDTH), (int)std::floorf(pos.y / Block::WIDTH), (int)std::floorf(pos.z / Block::WIDTH) };
	char* light = MobGenerator::GetWorld()->GetLightData(sampPos);
	char* skylight = MobGenerator::GetWorld()->GetSkyLightData(sampPos);

	//�����n�_�̖��邳�����ȉ�
	if (light && skylight && *light <= 4) {
		//���˓������󂯂Ă��Ȃ�
		if (second < sunsetSecond && second > sunriseSecond && *skylight > 7) {
			return;
		}

		//�v���C���[�̐����B
		Enemy* zombie = nullptr;
		if (CMath::RandomZeroToOne() < 0.18f) {
			zombie = MobGenerator::GetWorld()->CreateEntity<Creeper>();
		}
		else {
			zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
		}
		zombie->SetPos(pos);
	}
}