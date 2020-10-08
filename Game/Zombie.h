#pragma once
#include "Enemy.h"

class Zombie : public Enemy
{
public:
	Zombie();
	~Zombie();

	bool Start();
	void Update();

private:

};

