#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"
#include "Sun.h"

namespace {
	//日没。
	float sunsetSecond = 64800.0f;
	//日の出。
	float sunriseSecond = 21600.0f;
}

void ZombieGenerator::Init()
{
	//生成の様々なパラメータを設定する。
	GeneratorParameters* params = new GeneratorParameters();
	SetParametors(params);

	//生成するエンティティのタイプを設定する。
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
	//日没より現時刻が進んでいる、あるいは日の出より時刻が前だったら。
	//ゾンビを生成する。
	if (second > sunsetSecond || second < sunriseSecond) {
		//プレイヤーの生成。
		Zombie* zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
		zombie->SetPos(pos);
	}
}