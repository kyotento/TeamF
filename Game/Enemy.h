#pragma once

class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();

	//bool Start();
	//void Update();

	//�G�l�~�[�̃A�j���[�V�����B
	enum enAnimationClips {
		enAnimationClip_idle,
		enAnimationClip_move,
		enAnimationClip_tracking,
		enAnimationClip_attack,
		enAnimationClip_num
	};

	//�G�l�~�[�̏�ԁB
	enum enEnemyState {
		enEnemy_idle,
		enEnemy_move,
		enEnemy_tracking,
		enEnemy_attack,
		enEnemy_num,
	};


protected:

	CVector3 m_position = CVector3::Zero();			//���W�B
	CVector3 m_scale = CVector3::One();				//�v���C���[�̃X�P�[���B

	CQuaternion m_rot = CQuaternion::Identity();	//��]���B

	enAnimationClips m_animationClips[enAnimationClip_num];		//�A�j���[�V�����B
	enEnemyState m_enemyState = enEnemy_num;					//�G�l�~�[�̏�ԁB

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//�X�L�����f���B

};

