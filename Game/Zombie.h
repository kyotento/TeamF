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
	void Attack();		//�U���B
	void DamageVoice();

private:

	bool m_voiceFlag = false;

	int m_damageTimer = 0;	//�_���[�W���萶���^�C�}�[�B
	int m_oldHp = 0;		//1Frame�O�̗̑́B

	CVector3 GetDamageColPos();		//�_���[�W����̍��W�B

};

