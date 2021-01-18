#pragma once
#include "Entity.h"

struct GeneratorParameters
{
	//����min�`max�̃u���b�N�������v���C���[���痣�ꂽ���Ƀ��u�𐶐�������B
	//���̂Ƃ���Axz�̃u���b�N�����ǂꂾ������Ă��邩�v�Z���Ă��܂��B
	//�R�[�h�̓s����Amax / 2 > min �ɂ��Ƃ��ĂˁB
	//24�`128���ƃ��u�̐������킩��Â炢�̂Ŕ͈͂����߂Ă��܂��B
	//const int minDistanceMobGenerate = 24;
	//const int maxDistanceMobGenerate = 128;
	const int minDistanceMobGenerate = 20;
	const int maxDistanceMobGenerate = 50;
	//�ݒ肵���b����Ƀ��u�̐������J�n����B
	const float mobGenerateTimer = 7.0f;
	//��x�ɐ������郂�u�̍ő吔�B
	const int maxNumberMobGenerateOneTime = 4;
	//�͈͓��ɐ������郂�u�̍ő吔�B
	//�͈͓��Ƃ����̂́A�v���C���[����̋���������max�̃u���b�N���ȓ��̂��́B
	const int maxNumberMobRange = 10;
	//�S���[���h�ɐ������郂�u�̍ő吔�B
	const int maxNumberMob = 30;
};

class Player;
class World;
/// <summary>
/// ���u�𐶐����Ă����N���X�B
/// ���u�̓u���b�N�̂���ꏊ�ɂ�����������܂���B
/// </summary>
class MobGenerator : public IGameObject
{
public:
	~MobGenerator() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ���[���h���Z�b�g����B
	/// </summary>
	/// <param name="world">���[���h�B</param>
	void SetWorld(World* world)
	{
		m_world = world;
	}
protected:
	virtual void Init() = 0;
	
	/// <summary>
	/// �����̃p�����[�^��ݒ肷��B
	/// </summary>
	/// <param name="params"></param>
	void SetParametors(GeneratorParameters* params)
	{
		m_params = params;
	}
	/// <summary>
	/// ���̃C���X�^���X�𐶐����邩��ݒ肷��B
	/// </summary>
	/// <param name="typeEntity"></param>
	void SetTypeEntity(EnEntity typeEntity)
	{
		m_typeEntity = typeEntity;
	}
	/// <summary>
	/// ���u�𐶐�����B
	/// </summary>
	virtual void NewGOMob(const CVector3& pos) = 0;
	World* GetWorld()
	{
		return m_world;
	}
private:
	Player* GetPlayer()
	{
		if (m_player == nullptr)
		{
			m_player = FindGO<Player>(L"player");
		}
		return m_player;
	}
	/// <summary>
	/// ���u�𐶐�����B
	/// </summary>
	void GenerateMob();
	/// <summary>
	/// ���u�������ł����Ԃ����邩�ǂ����������B
	/// </summary>
	/// <param name="pos">�]���r�𐶐��\��̍��W�B</param>
	void SearchSpaceMobGenerate(CVector3& pos);
	/// <summary>
	/// ���u������͈͓̔��ɂǂꂾ�����݂��Ă��邩���ׂ�B
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W�B</param>
	void SearchNumberMobRange(CVector3& playerPos);
private:
	Player* m_player = nullptr;			//�v���C���[�B
	World* m_world;				//���[���h�B
	float m_timer = 0.0f;			//�^�C�}�[�B
	int m_numberMobRange = 0;		//�͈͓��ɑ��݂��Ă��郂�u�B
	int m_numberMob = 0;
	GeneratorParameters* m_params;
	EnEntity m_typeEntity;
};

