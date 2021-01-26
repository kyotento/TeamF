#pragma once
#include"Animals.h"
class Cow : public Animals
{
public:
	Cow();
	~Cow();
	bool Start();
	void Update();

private:
	bool m_voiceFlag = false;
	int a = 0;
};

