#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"

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
	//Zombie* zombie = NewGO<Zombie>();
	//�v���C���[�̐����B
	/*Zombie* zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
	zombie->SetPos(pos);*/
}