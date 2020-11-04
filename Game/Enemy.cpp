#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//�X�L�����f���𐶐��B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);

	if (m_player == nullptr) {			//�v���C���[�̃C���X�^���X���擾����Ă��Ȃ��Ƃ��B
		m_player = FindGO<Player>(L"player");		//�v���C���[�̃C���X�^���X���擾�B
	}
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

//������ǐՂ��鏈���B
void Enemy::Tracking()
{
	//�Ǐ]�����B
	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;		//���K������O�̒l���i�[�B
	m_direction.Normalize();
	m_direction.y = 0.f;
	if (m_oldDirection.Length() >= 1.01f * Block::WIDTH) {		//�v���C���[�ƈ�苗������Ă���Ƃ��B

		m_characonMove = m_direction * m_moveSpeed;
		m_position = m_characon.Execute(m_characonMove);
		m_skinModelRender->SetPos(m_position);

		m_enemyState = enEnemy_tracking;			//�ǐՏ�ԂɁB
	}
	else {
		m_enemyState = enEnemy_attack;				//�U����ԂɁB
	}

	//�v���C���[�̕��������������B
	m_rot.SetRotation(CVector3::AxisY(), atan2f(m_direction.x, m_direction.z));
	m_skinModelRender->SetRot(m_rot);
}

void Enemy::Fall()
{

}

//�G�l�~�[�̏�ԊǗ��B
void Enemy::StateManagement()
{
	switch (m_enemyState)
	{
	case enEnemy_idle:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_idle, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_move:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_tracking:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_tracking, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_attack:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_attack, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_fan:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAniamtionClip_fan, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	default:

		break;
	}
}