#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Zombie.h"

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
	//Zombie* zombie = NewGO<Zombie>();
	//プレイヤーの生成。
	/*Zombie* zombie = MobGenerator::GetWorld()->CreateEntity<Zombie>();
	zombie->SetPos(pos);*/
}