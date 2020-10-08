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
	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/zombie.cmo");
	m_position = { 35.f,15.f,35.f };
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale * 0.001f);
	m_skinModelRender->SetRot(m_rot);

	return true;
}

void Zombie::Update()
{

}
