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
	void Attack();		//�U���B
	void AttackSun();
private:

	bool m_voiceFlag = false;
	int m_damageTimer = 0;	//�_���[�W���萶���^�C�}�[�B
	CVector3 GetDamageColPos();		//�_���[�W����̍��W�B
	float m_sunDamageTimer = 0.0f;
	Sun* m_sun = nullptr;
};

