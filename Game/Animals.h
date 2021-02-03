#pragma once
#include"Player.h"
#include"GameMode.h"
#include "../physics/character/CCharacterController.h"
#include "Mob.h"

class Animals : public Mob
{
public:
	Animals(EnEntity enEntity = enEntity_None);
	virtual ~Animals();

	/// <summary>
	/// ��ԊǗ��B
	/// </summary>
	virtual void StateManagement();

	/// <summary>
	/// ���������B
	/// </summary>
	virtual void Fall();

	/// <summary>
	/// �W�����v�����B
	/// </summary>
	virtual void Jump();

	/// <summary>
	/// ��_���[�W�����B
	/// </summary>
	/// <param name="attackDamage">�U����</param>
	virtual void TakenDamage(int attackDamage);

	//��_�����̃_���[�W���B
	virtual void DamageVoice();

	/// <summary>
	/// �m�b�N�o�b�N�����B
	/// </summary>
	virtual void KnockBack();

	/// <summary>
	/// ���S���̏����B
	/// </summary>
	virtual void Death();

public:
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	CVector3 GetPos() const {
		return m_position;
	}
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	void SetPos(const CVector3& pos){
		m_position = pos;
		m_characon.SetPosition(pos);
		m_characonPos = pos;
	}

	//�X�P�[���̎w��B
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}

	//�����̏�ԁB
	enum enAnimalsState {
		enAnimals_idle,					//�ҋ@�B
		enAnimals_move,					//�ړ��B
		enAnimals_escape,					//���S�B
		enAnimals_death,					//���S�B
	};

protected:

	const wchar_t* m_damageVoice;		//�_���[�W���B
	const wchar_t* m_deathVoice;		//���S���B

	bool m_jumpFlag = false;			//�W�����v����ǂ����B
	bool m_isTakenDamage = false;		//�_���[�W���󂯂����B
	bool m_isInvincibleTime = false;	//���G���Ԃ��ǂ����B

	int m_hp = 0;						//�̗́B
	float m_knockBack = 1.f;			//�m�b�N�o�b�N���x�B
	float m_knoceBackY = 1.f;			//�m�b�N�o�b�NY���W�B

	const float m_moveSpeed = 100.0f;				//�ړ����x�B
	const float m_characonRadius = 40.f;			//�L�����R���̔��a�B
	const float m_characonHeight = 160.f;			//�L�����R���̍����B

	float m_fallSpeed = 0.8f;						//�������x�B
	float m_jumpSpeed = 1.f;						//�W�����v���x�B
	float m_jmpInitialVelocity = 13.f;				//�W�����v�̏����x�B
	float m_gravity = 0.65f;						//�d�́B
	float m_deathAddRot = 0.f;						//���S���̉�]�����B
	float m_knockBackTimer = 0.f;					//�m�b�N�o�b�N�^�C�}�[�B

	CVector3 m_position = CVector3::Zero();			//�����̍��W�B
	CVector3 m_scale = CVector3::One();				//�����̃X�P�[���B
	CVector3 m_characonPos = CVector3::Zero();		//�L�����R���̍��W�B
	CVector3 m_characonMove = CVector3::Zero();		//�L�����R���̈ړ���(������)�B
	CVector3 m_direction = CVector3::Zero();		//Animal���猩��Player�̌����B
	CVector3 m_oldDirection = CVector3::Zero();		//���K������O�̒l���i�[�B

	CQuaternion m_rot = CQuaternion::Identity();	//��]���B

	enAnimalsState m_animalState = enAnimals_idle;						//��ԁB

	MCCharaCon m_characon;								//�L�����R���B
	CRayTracingModelRender m_raytraceModel;								//���C�g�����f���B
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//�U����e����p�R���W�����B

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B
	Player* m_player = nullptr;									//�v���C���[�B
	GameMode* m_gameMode = nullptr;								//�Q�[�����[�h�B

	int a = 0;
};

