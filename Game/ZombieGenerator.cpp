#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"
#include "Creeper.h"
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

	float second = m_sun->GetSecond();
	IntVector3 sampPos = { (int)std::floorf(pos.x / Block::WIDTH), (int)std::floorf(pos.y / Block::WIDTH), (int)std::floorf(pos.z / Block::WIDTH) };
	char* light = MobGenerator::GetWorld()->GetLightData(sampPos);
	char* skylight = MobGenerator::GetWorld()->GetSkyLightData(sampPos);

	//生成地点の明るさが一定以下
	if (light && skylight && *light <= 4) {
		//直射日光を受けていない
		if (second < sunsetSecond && second > sunriseSecond && *skylight > 7) {
			return;
		}

		//プレイヤーの生成。
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