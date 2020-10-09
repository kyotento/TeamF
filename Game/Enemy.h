#pragma once
#include "Player.h"

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
	virtual void Tracking(/*const CVector3 pos*/);

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

protected:

	const float m_moveSpeed = 1.0f;					//�ړ����x�B

	CVector3 m_position = CVector3::Zero();			//���W�B
	CVector3 m_scale = CVector3::One();				//�v���C���[�̃X�P�[���B

	CQuaternion m_rot = CQuaternion::Identity();	//��]���B

	AnimationClip m_animationClips[enAnimationClip_num];		//�A�j���[�V�����B

	enEnemyState m_enemyState = enEnemy_num;					//�G�l�~�[�̏�ԁB

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B
	Player* m_player = nullptr;

};

