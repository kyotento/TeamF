#pragma once
#include "MobGenerator.h"

class Sun;
/// <summary>
/// �]���r�𐶐����Ă����N���X�B
/// �]���r�̓u���b�N�̂���ꏊ�ɂ�����������܂���B
/// </summary>
class ZombieGenerator : public MobGenerator
{
public:
	void Init() override;
	void NewGOMob(const CVector3& pos) override;
private:
	Sun* m_sun = nullptr;
};

