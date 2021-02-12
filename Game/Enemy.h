#pragma once
#include "Player.h"
#include "../physics/character/CCharacterController.h"
#include "GameMode.h"
#include "Mob.h"
#include "Game.h"
class World;
class Enemy : public Mob
{
public:
	Enemy(EnEntity enEntity = enEntity_None);
	virtual ~Enemy();

	/// <summary>
	/// ������ǐՂ��鏈���B
	/// </summary>
	/// <param name="pos">�ǐՂ�����Enemy�̍��W</param>
	/// �v���C���[��Ǐ]����Enemy�̎q�N���X�ŌĂ�ł����Ă��������B
	virtual void Tracking();

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
	/// ���z����̃_���[�W
	/// </summary>
	virtual void AttackSun();

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

	//! @brief ���W���擾�B
	CVector3 GetPos() const override{
		return m_position;
	}

	//! @brief ���W��ݒ�B
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
		m_characonPos = pos;
	}

	/// <summary>
	/// �G���e�B�e�B�̗L���ؑ֎��̏���
	/// </summary>
	/// <param name="enable"></param>
	void SetEnableEntityInner(bool enable)override {
		m_damageCollision->SetEnable(enable);
		m_skinModelRender->SetEnable(enable);
	}

	//�G�l�~�[�̃A�j���[�V�����B
	enum enAnimationClips {
		enAnimationClip_idle,			//�ҋ@�B
		enAnimationClip_move,			//�ړ��B
		enAnimationClip_tracking,		//�ǐՁB
		enAnimationClip_attack,			//�U���B
		enAniamtionClip_fan,			//����B
		enAnimationClip_num				//�A�j���[�V�����̐��B
	};

	//�G�l�~�[�̏�ԁB
	enum enEnemyState {
		enEnemy_idle,					//�ҋ@�B
		enEnemy_move,					//�ړ��B
		enEnemy_tracking,				//�ǐՁB
		enEnemy_attack,					//�U���B
		enEnemy_fan,					//����B		
		enEnemy_death,					//���񂾂Ƃ��B
		enEnemy_num,					//�G�l�~�[�̏�Ԃ̐��B
	};

	//�X�P�[���̎w��B
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}

protected:

	const wchar_t* m_damageVoice;		//�_���[�W���B
	const wchar_t* m_deathVoice;		//���S���B

	bool m_jumpFlag = false;			//�W�����v����ǂ����B
	bool m_isTakenDamage = false;		//�_���[�W���󂯂����B
	//todo �����_�ł́A�_���[�W�t���O���g���΂����B
	bool m_isInvincibleTime = false;	//���G���Ԃ��ǂ����B

	int m_hp = 0;				//�̗́B
	int m_attackPow = 0;		//�U���́B
	float m_exp = 0.f;			//�擾�o���l�ʁB
	float m_knockBack = 1.f;	//�m�b�N�o�b�N���x�B
	float m_knoceBackY = 1.f;	//�m�b�N�o�b�NY���W�B

	const float m_moveSpeed = 100.0f;				//�ړ����x�B
	const float m_characonRadius = 40.f;			//�L�����R���̔��a�B
	const float m_characonHeight = 160.f;			//�L�����R���̍����B
	const float m_interpolateTimeSec = 0.3f;		//�A�j���[�V�����؂�ւ����̃A�j���[�V������Ԏ��ԁB
	const float m_animSpeed = 1.0f;					//�A�j���[�V�����̑��x�B
	float m_fallSpeed = 0.8f;						//�������x�B
	float m_jumpSpeed = 1.f;						//�W�����v���x�B
	float m_jmpInitialVelocity = 13.f;				//�W�����v�̏����x�B
	float m_gravity = 0.65f;						//�d�́B
	float m_deathAddRot = 0.f;						//���S���̉�]�����B
	float m_knockBackTimer = 0.f;					//�m�b�N�o�b�N�^�C�}�[�B

	CVector3 m_position = CVector3::Zero();			//�G�l�~�[�̍��W�B
	CVector3 m_scale = CVector3::One();				//�v���C���[�̃X�P�[���B
	CVector3 m_characonPos = CVector3::Zero();		//�L�����R���̍��W�B
	CVector3 m_characonMove = CVector3::Zero();		//�L�����R���̈ړ���(������)�B
	CVector3 m_direction = CVector3::Zero();		//Enemy���猩��Player�̌����B
	CVector3 m_oldDirection = CVector3::Zero();		//���K������O�̒l���i�[�B

	CQuaternion m_rot = CQuaternion::Identity();	//��]���B

	AnimationClip m_animationClips[enAnimationClip_num];		//�A�j���[�V�����B

	enEnemyState m_enemyState = enEnemy_num;					//�G�l�~�[�̏�ԁB

	MCCharaCon m_characon;												//�L�����R���B
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//�U����e����p�R���W�����B

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B
	CRayTracingModelRender m_raytraceModel;						//���C�g�����f���B
	Player* m_player = nullptr;									//�v���C���[�B
	GameMode* m_gameMode = nullptr;								//�Q�[�����[�h�B
	Game* m_game = nullptr;										//Game�B

	float m_sunDamageTimer = 0.0f;
	Sun* m_sun = nullptr;
};

