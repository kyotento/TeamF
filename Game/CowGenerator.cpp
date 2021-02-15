#include "stdafx.h"
#include "CowGenerator.h"
#include"Cow.h"

void CowGenerator::Init()
{
	//生成の様々なパラメータを設定する。
	GeneratorParameters* params = new GeneratorParameters();
	params->mobGenerateTimer = 4.0f;		//生成にかかる時間。
	params->maxNumberMobRange = 4;			//一度に生成する数。
	params->maxNumberMob = 40;				//牛の生成上限
	SetParametors(params);

	//生成するエンティティのタイプを設定する。
	SetTypeEntity(enEntity_Cow);
}

void CowGenerator::NewGOMob(const CVector3& pos)
{
	Cow* cow = MobGenerator::GetWorld()->CreateEntity<Cow>();
	cow->SetPos(pos);
}