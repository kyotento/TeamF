#pragma once
#include "Enemy.h"

class World;
class Zombie : public Enemy
{
public:
	Zombie();
	~Zombie();

	bool Start();
	void Update();
	void Attack();		//�U���B

private:

	bool m_voiceFlag = false;
	int m_damageTimer = 0;	//�_���[�W���萶���^�C�}�[�B
	CVector3 GetDamageColPos();		//�_���[�W����̍��W�B

};

