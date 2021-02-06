#pragma once
#include "MobGenerator.h"

class Sun;
/// <summary>
/// ゾンビを生成していくクラス。
/// ゾンビはブロックのある場所にしか生成されません。
/// </summary>
class ZombieGenerator : public MobGenerator
{
public:
	void Init() override;
	void NewGOMob(const CVector3& pos) override;
private:
	Sun* m_sun = nullptr;
};

