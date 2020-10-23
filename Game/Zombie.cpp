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
	//m_position = { 35.f,15.f,35.f };
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);

	m_characon.SetPosition(m_position);

	return true;
}

void Zombie::Update()
{
	Tracking();				//プレイヤーを追跡する処理。
	StateManagement();		//状態管理。
}
