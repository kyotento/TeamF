#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
public:
	Boss();
	~Boss();

	bool Start();
	void Update();
	void Attack();		//�U���B


	void TakenDamage(int attackDamage)override;

private:
	void Move();

	bool m_voiceFlag = false;
	int m_damageTimer = 0;	//�_���[�W���萶���^�C�}�[�B
	CVector3 GetDamageColPos();		//�_���[�W����̍��W�B	

	float m_stateTimer = 0.0f;
};