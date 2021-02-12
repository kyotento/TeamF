#pragma once
#include "Enemy.h"

class Creeper : public Enemy
{
public:
	Creeper() : Enemy(enEntity_Zombie) {}

	bool Start()override;
	void Update()override;

	void Attack();

private:
	int m_bombCount = 0;
};

