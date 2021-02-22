#pragma once
#include "MobGenerator.h"

class Sun;

class BossGenerator : public MobGenerator
{
public:
	void Init() override;
	void NewGOMob(const CVector3& pos) override;
	void PreUpdate()override;
private:
	Sun* m_sun = nullptr;
	int m_runCnt = 1;//s‰ñ”
	float m_bossTimerSec = 0.0f;
};
