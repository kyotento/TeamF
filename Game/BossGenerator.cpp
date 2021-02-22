#include "stdafx.h"
#include "BossGenerator.h"
#include "Boss.h"
#include "Sun.h"

namespace {
	//日没。
	float sunsetSecond = 64800.0f;
	//日の出。
	float sunriseSecond = 21600.0f;
}

void BossGenerator::Init()
{
	//生成の様々なパラメータを設定する。
	GeneratorParameters* params = new GeneratorParameters();
	params->maxNumberMobGenerateOneTime = 10;
	params->maxNumberMobRange = 1;
	params->maxNumberMob = 1;
	params->mobGenerateTimer = 60.0f * 1.0f;//1分に一回生成処理
	SetParametors(params);

	//生成するエンティティのタイプを設定する。
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

	//昼間のみ湧く
	float second = m_sun->GetSecond();
	if (!(second > sunriseSecond && second < sunsetSecond)) {
		return;
	}

	//確率判定
	if (m_runCnt / 20.0f <= CMath::RandomZeroToOne()) {
		return;
	}

	//プレイヤーの生成。
	Enemy* boss = MobGenerator::GetWorld()->CreateEntity<Boss>();
	boss->SetPos(pos);

	m_runCnt = 0;
}

void BossGenerator::PreUpdate() {
	m_bossTimerSec += GetDeltaTimeSec();
	if (m_bossTimerSec >= 60.0f * 1.0f)//一分に一回
	{
		m_bossTimerSec = 0.0f;

		/*
		if (m_sun == nullptr) {
			m_sun = FindGO<Sun>();
			if (m_sun == nullptr) {
				return;
			}
		}

		//昼間のみ増加
		float second = m_sun->GetSecond();
		if (!(second > sunriseSecond && second < sunsetSecond)) {
			return;
		}
		*/

		//カウント増加
		m_runCnt++;
	}
}