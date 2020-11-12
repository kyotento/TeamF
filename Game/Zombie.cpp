#include "stdafx.h"
#include "Zombie.h"


Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

bool Zombie::Start()
{
	//アニメーションの初期化。
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);

	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/zombie.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);

	m_characon.SetPosition(m_position);

	m_attackPow = 1;

	return true;
}

void Zombie::Update()
{
	//todo 仮。
	if (m_position.y <= 3.f) {
		return;
	}
	Tracking();				//プレイヤーを追跡する処理。
	StateManagement();		//状態管理。
	Attack();				//攻撃。
}

void Zombie::Attack()
{
	if (m_enemyState == enEnemy_attack) {	//攻撃状態のとき。
		//todo アニメーションのフレーム数を取得したい or　アニメーションイベントの作成。
		m_damageTimer += 1;		//ダメージ判定用タイマーを加算。
		if (m_damageTimer == 7) {		//ダメージ判定用のフレームに到達した時。
			SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
			attackCol->CreateBox(GetDamageColPos(), m_rot, Block::WIDTH / 4);
			attackCol->SetTimer(0);		//寿命１フレーム。
			attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
				if (param.EqualName(L"CPlayer")) {			//名前検索。
					m_player->TakenDamage(m_attackPow);		//プレイヤーにダメージを与える。
				}
			});
		}
		if (m_damageTimer == 9){
			m_damageTimer = 0;			//タイマーリセッツ。
		}
	}	
	else {			//攻撃状態でないとき。
		m_damageTimer = 0;
	}
}

//ゾンビのダメージ判定の座標を取得。
CVector3 Zombie::GetDamageColPos()
{
	Bone* rightHandBone = m_skinModelRender->FindBone(L"Bone009");		//右手の骨。
	Bone* leftHandBone = m_skinModelRender->FindBone(L"Bone006");		//左手の骨。

	const CVector3 rightHandBonePos = rightHandBone->GetPosition();		//右手の骨の座標。
	const CVector3 leftHandBonePos = leftHandBone->GetPosition();		//左手の骨の座標。

	CVector3 DamageColPos = (rightHandBonePos + leftHandBonePos) / 2;	//ダメージ判定の座標。

	return DamageColPos;
}