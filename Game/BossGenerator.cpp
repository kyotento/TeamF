#include "stdafx.h"
#include "BossGenerator.h"
#include "Boss.h"
#include "Sun.h"

namespace {
	//���v�B
	float sunsetSecond = 64800.0f;
	//���̏o�B
	float sunriseSecond = 21600.0f;
}

void BossGenerator::Init()
{
	//�����̗l�X�ȃp�����[�^��ݒ肷��B
	GeneratorParameters* params = new GeneratorParameters();
	params->maxNumberMobGenerateOneTime = 10;
	params->maxNumberMobRange = 1;
	params->maxNumberMob = 1;
	params->mobGenerateTimer = 60.0f * 1.0f;//1���Ɉ�񐶐�����
	SetParametors(params);

	//��������G���e�B�e�B�̃^�C�v��ݒ肷��B
	SetTypeEntity(enEntity_Zombie);
}

void BossGenerator::NewGOMob(const CVector3& pos)
{
	if (m_sun == nullptr){
		m_sun = FindGO<Sun>();
		if (m_sun == nullptr) {
			return;
		}
	}

	//���Ԃ̂ݗN��
	float second = m_sun->GetSecond();
	if (!(second > sunriseSecond && second < sunsetSecond)) {
		return;
	}

	//�m������
	if (m_runCnt / 20.0f <= CMath::RandomZeroToOne()) {
		return;
	}

	//�v���C���[�̐����B
	Enemy* boss = MobGenerator::GetWorld()->CreateEntity<Boss>();
	boss->SetPos(pos);

	m_runCnt = 0;
}

void BossGenerator::PreUpdate() {
	m_bossTimerSec += GetDeltaTimeSec();
	if (m_bossTimerSec >= 60.0f * 1.0f)//�ꕪ�Ɉ��
	{
		m_bossTimerSec = 0.0f;

		/*
		if (m_sun == nullptr) {
			m_sun = FindGO<Sun>();
			if (m_sun == nullptr) {
				return;
			}
		}

		//���Ԃ̂ݑ���
		float second = m_sun->GetSecond();
		if (!(second > sunriseSecond && second < sunsetSecond)) {
			return;
		}
		*/

		//�J�E���g����
		m_runCnt++;
	}
}