#pragma once
#include"Animals.h"
class Cow : public Animals
{
	Cow();
	~Cow();
	bool Start();
	void Update();
private:
	bool m_voiceFlag = false;
};

