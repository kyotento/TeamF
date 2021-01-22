#include "stdafx.h"
#include "CowGenerator.h"
#include"Cow.h"

void CowGenerator::Init()
{
	//�����̗l�X�ȃp�����[�^��ݒ肷��B
	GeneratorParameters* params = new GeneratorParameters();
	params->mobGenerateTimer = 15.0f;		//�����ɂ����鎞�ԁB
	params->maxNumberMobRange = 2;			//��x�ɐ������鐔�B
	params->maxNumberMob = 10;				//���̐������
	SetParametors(params);

	//��������G���e�B�e�B�̃^�C�v��ݒ肷��B
	SetTypeEntity(enEntity_Cow);
}

void CowGenerator::NewGOMob(const CVector3& pos)
{
	Cow* cow = MobGenerator::GetWorld()->CreateEntity<Cow>();
	cow->SetPos(pos);
}