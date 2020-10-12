#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//�X�L�����f���𐶐��B
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
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
void Enemy::Tracking(/*const CVector3 pos*/)
{
	//�Ǐ]�����B
	CVector3 direction;			//Enemy���猩��Player�̌����B
	direction = (m_player->GetPosition() - m_characonPos);
	direction.y = 0.f;			//�����͕s�v�Ȃ̂łO�����B
	if (direction.Length() >= 1.f * Block::WIDTH) {
		direction.Normalize();
		m_characonPos += direction * m_moveSpeed;
		m_characon.Execute(m_characonPos);
		m_position = m_characonPos;
		m_skinModelRender->SetPos(m_position);
	}
	//�v���C���[�̕��������������B
	m_rot.SetRotation(CVector3::AxisY(), atan2f(direction.x, direction.z));
	m_skinModelRender->SetRot(m_rot);
}