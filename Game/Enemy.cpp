#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	//スキンモデルを生成。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
	//キャラコンの初期化。
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);

	//プレイヤーのインスタンスを取得。
	if (m_player == nullptr) {			
		m_player = FindGO<Player>(L"player");		
	}
	//ゲームモードのインスタンスを取得。
	if (m_gameMode == nullptr) {
		m_gameMode = FindGO<GameMode>(L"gamemode");
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

	Jump();		//ジャンプ処理。

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

//落下処理。
void Enemy::Fall()    
{
	//自由落下。
	if (!m_characon.IsOnGround()) {
		m_fallSpeed += 0.5f;
	}
	else
	{
		m_fallSpeed = 1.f;
	}

	m_direction.y -= m_fallSpeed;
}

//ジャンプ。
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