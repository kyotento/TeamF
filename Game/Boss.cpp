#include "stdafx.h"
#include "Boss.h"

Boss::Boss() : Enemy(enEntity_Zombie)
{
}

Boss::~Boss()
{
}

bool Boss::Start()
{
	//アニメーションの初期化。
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);

	m_scale *= 1.25f;

	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/zombie.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//レイトレモデル初期化
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = 20;					//体力の設定。
	m_attackPow = 10;			//攻撃力の設定。
	m_exp = 7.0f;				//取得経験値量。
	m_knockBack = 1.7f;			//ノックバック倍率。
	m_moveSpeed *= 7.0f;		//移動速度
	m_attackRenge *= 2.0f;

	m_damageVoice = L"Resource/soundData/enemy/zombiedamage.wav";
	m_deathVoice = L"Resource/soundData/enemy/zombiedeath.wav";

	//アイテムドロップ
	SetDropItemId(enCube_WoGBlock);
	SetChanceDropping(100);
	//レアドロップ
	SetDropItemId(enCube_WoGBlock, true);
	SetChanceDropping(5, true);

	return true;
}

void Boss::Update()
{
	//EscMenuが開いているとき更新を止める。
	if (m_game->GetIsEscMenu()) {
		return;
	}

	//エネミーが存在しているときのみ。
	if (m_enemyState != enEnemy_death) {

		if (m_stateTimer > 0.0f) {
			m_stateTimer -= GetDeltaTimeSec();
		}

		if (m_stateTimer < FLT_EPSILON) {
			if (m_enemyState == enEnemy_fan) {
				m_enemyState = enEnemy_attack;
				m_stateTimer = 0.10f;//攻撃時間
			}
		}

		Move();

		//サバイバルのとき。
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();				//攻撃。
		}

		StateManagement();		//状態管理。
	}
	KnockBack();	//ノックバック処理。
	Death();		//死亡判定
	//AttackSun();	//太陽によるダメージ。
}

//攻撃処理。
void Boss::Attack()
{
	if (m_enemyState == enEnemy_attack) {	//攻撃状態のとき。
		m_damageTimer += 1;		//ダメージ判定用タイマーを加算。
		if (m_damageTimer == 7) {		//ダメージ判定用のフレームに到達した時。
			SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
			attackCol->CreateBox(GetDamageColPos(), m_rot, Block::WIDTH / 4);
			attackCol->SetTimer(0);		//寿命１フレーム。
			attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
				if (param.EqualName(L"CPlayer")) {			//名前検索。
					CVector3 direction = GetDamageColPos() - GetPos();
					m_player->TakenDamage(m_attackPow, direction, true);		//プレイヤーにダメージを与える。
				}
			});
		}
		if (m_damageTimer == 9) {
			m_damageTimer = 0;			//タイマーリセッツ。
		}
	}
	else {			//攻撃状態でないとき。
		m_damageTimer = 0;
	}
}

//ゾンビのダメージ判定の座標を取得。
CVector3 Boss::GetDamageColPos()
{
	Bone* rightHandBone = m_skinModelRender->FindBone(L"Bone009");		//右手の骨。
	Bone* leftHandBone = m_skinModelRender->FindBone(L"Bone006");		//左手の骨。

	const CVector3 rightHandBonePos = rightHandBone->GetPosition();		//右手の骨の座標。
	const CVector3 leftHandBonePos = leftHandBone->GetPosition();		//左手の骨の座標。

	CVector3 DamageColPos = (rightHandBonePos + leftHandBonePos) / 2;	//ダメージ判定の座標。

	return DamageColPos;
}

void Boss::TakenDamage(int attackDamage)
{
	//if (m_enemyState == enEnemy_attack) {
	//	Enemy::TakenDamage(attackDamage);
	//}
	//else {
		m_enemyState = enEnemy_idle;
		m_stateTimer = 0.5f;

		m_isTakenDamage = true;			//ダメージフラグを返す。
	//}
}

void Boss::Move()
{
	//追従処理。
	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;		//正規化する前の値を格納。

	if (m_stateTimer < FLT_EPSILON) {
		m_direction.Normalize();
		m_direction.y = 0.f;

		//ジャンプ処理。
		if (m_characon.IsContactWall() && m_characon.IsOnGround())
		{
			m_jumpFlag = true;
		}
		if (m_jumpFlag) {
			m_direction.y += m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity;

			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity) {
				m_jumpFlag = false;
				m_jmpInitialVelocity = 13.f;
			}
		}
	}
	else {
		m_direction = CVector3::Zero();
	}

	//落下
	Fall();

	//プレイヤーと一定距離離れていて、ノックバックしていないとき。
	if (!m_isTakenDamage) {
		if (m_oldDirection.Length() >= m_attackRenge) {
			m_enemyState = enEnemy_tracking;			//追跡状態に。
		}
		else {
			m_direction.x = m_direction.z = 0.0f;
			//m_enemyState = enEnemy_fan;				//攻撃状態に。
			//m_stateTimer = 0.8f;
		}
	}

	//モデル、キャラコン、当たり判定用の座標を更新。
	m_characonMove = m_direction * m_moveSpeed;
	m_position = m_characon.Execute(m_characonMove);
	m_skinModelRender->SetPos(m_position);
	CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//当たり判定の座標。
	m_damageCollision->SetPosition(colPos);

	//プレイヤーの方向を向く処理。
	m_rot.SetRotation(CVector3::AxisY(), atan2f(m_direction.x, m_direction.z));
	m_skinModelRender->SetRot(m_rot);
}