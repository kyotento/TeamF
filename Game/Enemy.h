#pragma once
#include "Player.h"
#include "../physics/character/CCharacterController.h"

class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	/// <summary>
	/// ������ǐՂ��鏈���B
	/// </summary>
	/// <param name="pos">�ǐՂ�����Enemy�̍��W</param>
	/// �v���C���[��Ǐ]����Enemy�̎q�N���X�ŌĂ�ł����Ă��������B
	virtual void Tracking();

	//��ԊǗ��B
	virtual void StateManagement();

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
		enEnemy_num,					//�G�l�~�[�̏�Ԃ̐��B
	};
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
protected:

	const float m_moveSpeed = 100.0f;					//�ړ����x�B
	const float m_characonRadius = 40.f;			//�L�����R���̔��a�B
	const float m_characonHeight = 160.f;			//�L�����R���̍����B
	const float m_interpolateTimeSec = 0.3f;		//�A�j���[�V�����؂�ւ����̃A�j���[�V������Ԏ��ԁB
	const float m_animSpeed = 1.0f;					//�A�j���[�V�����̑��x�B

	CVector3 m_position = CVector3::Zero();			//�G�l�~�[�̍��W�B
	CVector3 m_scale = CVector3::One();				//�v���C���[�̃X�P�[���B
	CVector3 m_characonPos = CVector3::Zero();		//�L�����R���̍��W�B
	CVector3 m_characonMove = CVector3::Zero();		//�L�����R���̈ړ���(������)�B

	CQuaternion m_rot = CQuaternion::Identity();	//��]���B

	AnimationClip m_animationClips[enAnimationClip_num];		//�A�j���[�V�����B

	enEnemyState m_enemyState = enEnemy_num;					//�G�l�~�[�̏�ԁB

	CCharacterControllerType2 m_characon;							//�L�����R���B

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B
	Player* m_player = nullptr;

};

