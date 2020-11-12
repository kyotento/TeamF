#pragma once
#include "Enemy.h"

class Zombie : public Enemy
{
public:
	Zombie();
	~Zombie();

	bool Start();
	void Update();
	void Attack();		//�U���B

private:

	int m_damageTimer = 0;	//�_���[�W���萶���^�C�}�[�B

	CVector3 GetDamageColPos();		//�_���[�W����̍��W�B

};

