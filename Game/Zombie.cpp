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
	//�A�j���[�V�����̏������B
	m_animationClips[enAnimationClip_idle].Load(L"Assets/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Assets/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Assets/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Assets/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Assets/animData/zombie_fan.tka", true);

	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/zombie.cmo", m_animationClips, enAnimationClip_num);
	m_position = { 35.f,15.f,35.f };
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale * 0.01f * Block::WIDTH);
	m_skinModelRender->SetRot(m_rot);

	return true;
}

void Zombie::Update()
{
	Tracking();				//�v���C���[��ǐՂ��鏈���B
	StateManagement();		//��ԊǗ��B
}
