#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(World* world) : Entity(world)
{
	//スキンモデルを生成。
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_characonPos = m_position;
	//キャラコンの初期化。
	m_characon.Init(m_characonRadius, m_characonHeight, m_characonPos);
	//被弾判定用コリジョン。
	m_damageCollision = std::make_unique<SuicideObj::CCollisionObj>();
	CVector3 colPos = (m_position.x, m_position.y + Block::WIDTH, m_position.z);		//コリジョン座標。
	m_damageCollision->CreateCapsule(colPos, m_rot, m_characonRadius, m_characonHeight);
	m_damageCollision->SetTimer(enNoTimer);				//寿命無し。
	m_damageCollision->SetName(L"CEnemy");
	m_damageCollision->SetClass(this);					//クラスのポインタを取得。
	m_damageCollision->SetIsHurtCollision(true);		//自分から判定をとらない。
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

		//モデル、キャラコン、当たり判定用の座標を更新。
		m_characonMove = m_direction * m_moveSpeed;
		m_position = m_characon.Execute(m_characonMove);
		m_skinModelRender->SetPos(m_position);
		CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//当たり判定の座標。
		m_damageCollision->SetPosition(colPos);

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

//被ダメージ処理。
void Enemy::TakenDamage(int attackDamage)
{
	if (m_hp >= 0) {		//HPがあるとき。
		m_hp -= attackDamage;
		//体力を0未満にしない。
		if (m_hp <= 0) {
			m_hp = 0;
		}
		MessageBox(nullptr, "残りHP%d", "攻撃された", MB_ICONASTERISK);
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