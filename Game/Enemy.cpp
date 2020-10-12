#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//スキンモデルを生成。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	//キャラコンの初期化。
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);

	if (m_player == nullptr) {			//プレイヤーのインスタンスが取得されていないとき。
		m_player = FindGO<Player>(L"player");		//プレイヤーのインスタンスを取得。
	}
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}

//何かを追跡する処理。
void Enemy::Tracking(/*const CVector3 pos*/)
{
	//追従処理。
	CVector3 direction;			//Enemyから見たPlayerの向き。
	direction = (m_player->GetPosition() - m_characonPos);
	direction.y = 0.f;			//高さは不要なので０を代入。
	if (direction.Length() >= 1.f * Block::WIDTH) {
		direction.Normalize();
		m_characonPos += direction * m_moveSpeed;
		m_characon.Execute(m_characonPos);
		m_position = m_characonPos;
		m_skinModelRender->SetPos(m_position);
	}
	//プレイヤーの方向を向く処理。
	m_rot.SetRotation(CVector3::AxisY(), atan2f(direction.x, direction.z));
	m_skinModelRender->SetRot(m_rot);
}