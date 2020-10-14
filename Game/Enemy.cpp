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
	CVector3 direction;			//Enemy���猩��Player�̌����B
	direction = (m_player->GetPosition() - m_characonPos);
	direction.Normalize();
	m_characonPos += direction * m_moveSpeed;
	direction.y = 0.f;			//�����͕s�v�Ȃ̂łO�����B

	if (direction.Length() >= 1.f * Block::WIDTH) {		//�v���C���[�ƈ�苗������Ă���Ƃ��B

		m_position = m_characon.Execute(m_characonPos);
		m_skinModelRender->SetPos(m_position);

		m_enemyState = enEnemy_tracking;			//�ǐՏ�ԂɁB
	}
	else {
		m_enemyState = enEnemy_attack;				//�U����ԂɁB
	}
	//�v���C���[�̕��������������B
	m_rot.SetRotation(CVector3::AxisY(), atan2f(direction.x, direction.z));
	m_skinModelRender->SetRot(m_rot);
}

//�G�l�~�[�̏�ԊǗ��B
void Enemy::StateManagement()
{
	switch (m_enemyState)
	{
	enEnemy_idle:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_idle, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	enEnemy_move:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	enEnemy_tracking:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_tracking, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	enEnemy_attack:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_attack, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	enEnemy_fan:

		//�A�j���[�V�����̍Đ��B
		m_skinModelRender->GetAnimCon().Play(enAniamtionClip_fan, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	default:

		break;
	}
}