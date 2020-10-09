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

//何かを追跡する処理。
void Enemy::Tracking(/*const CVector3 pos*/)
{
	if (m_player == nullptr) {			//プレイヤーのインスタンスが取得されていないとき。
		m_player = FindGO<Player>(L"player");		//プレイヤーのインスタンスを取得。
	}

	CVector3 direction;			//Enemyから見たPlayerの向き。
	direction = (m_player->GetPosition() - m_position);
	direction.y = 0.f;			//高さは不要なので０を代入。
	direction.Normalize();
	m_position += direction * m_moveSpeed;		
	m_skinModelRender->SetPos(m_position);

}