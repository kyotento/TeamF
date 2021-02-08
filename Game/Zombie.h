#pragma once
#include "Enemy.h"

class World;
class Sun;
class Zombie : public Enemy
{
public:
	Zombie();
	~Zombie();

	bool Start();
	void Update();
	void Attack();		//攻撃。
	void AttackSun();
private:

	bool m_voiceFlag = false;
	int m_damageTimer = 0;	//ダメージ判定生成タイマー。
	CVector3 GetDamageColPos();		//ダメージ判定の座標。
	float m_sunDamageTimer = 0.0f;
	Sun* m_sun = nullptr;
};

