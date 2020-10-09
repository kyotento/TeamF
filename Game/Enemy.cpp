#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

//������ǐՂ��鏈���B
void Enemy::Tracking(/*const CVector3 pos*/)
{
	if (m_player == nullptr) {			//�v���C���[�̃C���X�^���X���擾����Ă��Ȃ��Ƃ��B
		m_player = FindGO<Player>(L"player");		//�v���C���[�̃C���X�^���X���擾�B
	}

	CVector3 direction;			//Enemy���猩��Player�̌����B
	direction = (m_player->GetPosition() - m_position);
	direction.y = 0.f;			//�����͕s�v�Ȃ̂łO�����B
	direction.Normalize();
	m_position += direction * m_moveSpeed;		
	m_skinModelRender->SetPos(m_position);

}