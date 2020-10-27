#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//スキンモデルを生成。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
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
void Enemy::Tracking()
{
	//追従処理。
	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;		//正規化する前の値を格納。
	m_direction.Normalize();
	m_direction.y = 0.f;
	if (m_oldDirection.Length() >= 1.01f * Block::WIDTH) {		//プレイヤーと一定距離離れているとき。

		m_characonMove = m_direction * m_moveSpeed;
		m_position = m_characon.Execute(m_characonMove);
		m_skinModelRender->SetPos(m_position);

		m_enemyState = enEnemy_tracking;			//追跡状態に。
	}
	else {
		m_enemyState = enEnemy_attack;				//攻撃状態に。
	}

	//プレイヤーの方向を向く処理。
	m_rot.SetRotation(CVector3::AxisY(), atan2f(m_direction.x, m_direction.z));
	m_skinModelRender->SetRot(m_rot);
}

void Enemy::Fall()
{

}

//エネミーの状態管理。
void Enemy::StateManagement()
{
	switch (m_enemyState)
	{
	case enEnemy_idle:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_idle, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_move:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_move, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_tracking:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_tracking, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_attack:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAnimationClip_attack, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	case enEnemy_fan:

		//アニメーションの再生。
		m_skinModelRender->GetAnimCon().Play(enAniamtionClip_fan, m_interpolateTimeSec);
		m_skinModelRender->GetAnimCon().SetSpeed(m_animSpeed);

		break;
	default:

		break;
	}
}