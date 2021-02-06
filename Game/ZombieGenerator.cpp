#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"
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
		//�v���C���[�̐����B
		Zombie* zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
		zombie->SetPos(pos);
	}
}