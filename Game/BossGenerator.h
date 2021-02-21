#pragma once
#include "MobGenerator.h"

class Sun;

class BossGenerator : public MobGenerator
{
public:
	void Init() override;
	void NewGOMob(const CVector3& pos) override;
private:
	Sun* m_sun = nullptr;
	int m_runCnt = 0;//s‰ñ”
};
