#pragma once
#include "MobGenerator.h"

class CowGenerator : public MobGenerator
{
public:
	void Init() override;
	void NewGOMob(const CVector3& pos) override;
private:
};

