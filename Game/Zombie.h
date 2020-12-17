#pragma once
#include "Enemy.h"

class World;
class Zombie : public Enemy
{
public:
	Zombie(World* world);
	~Zombie();

	bool Start();
	void Update();
	void Attack();		//攻撃。
	void DamageVoice();

private:

	bool m_voiceFlag = false;

	int m_damageTimer = 0;	//ダメージ判定生成タイマー。
	int m_oldHp = 0;		//1Frame前の体力。

	CVector3 GetDamageColPos();		//ダメージ判定の座標。

};

