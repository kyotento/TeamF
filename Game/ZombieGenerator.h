#pragma once
#include "MobGenerator.h"

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
};

