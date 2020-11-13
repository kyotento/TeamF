#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//�X�L�����f���𐶐��B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
	//�L�����R���̏������B
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);

	//�v���C���[�̃C���X�^���X���擾�B
	if (m_player == nullptr) {			
		m_player = FindGO<Player>(L"player");		
	}
	//�Q�[�����[�h�̃C���X�^���X���擾�B
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
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

	Jump();		//�W�����v�����B

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

//���������B
void Enemy::Fall()    
{
	//���R�����B
	if (!m_characon.IsOnGround()) {
		m_fallSpeed += 0.5f;
	}
	else
	{
		m_fallSpeed = 1.f;
	}

	m_direction.y -= m_fallSpeed;
}

//�W�����v�B
void Enemy::Jump()
{
	if (m_characon.IsContactWall() && m_characon.IsOnGround())
	{
		flag = true;
	}

	if (flag) {
		m_direction.y += m_jmpInitialVelocity;
		m_jmpInitialVelocity -= m_gravity;

		if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity) {
			flag = false;
			m_jmpInitialVelocity = 13.f;
		}
	}
	else {
		Fall();
	}
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