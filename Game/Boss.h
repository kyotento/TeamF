#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
public:
	Boss();
	~Boss();

	bool Start();
	void Update();
	void Attack();		//攻撃。


	void TakenDamage(int attackDamage)override;

private:
	void Move();

	bool m_voiceFlag = false;
	int m_damageTimer = 0;	//ダメージ判定生成タイマー。
	CVector3 GetDamageColPos();		//ダメージ判定の座標。	

	float m_stateTimer = 0.0f;
};