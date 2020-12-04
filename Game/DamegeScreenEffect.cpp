#include "stdafx.h"
#include "DamegeScreenEffect.h"

bool DamegeScreenEffect::Start() {
	m_sprite.Init(L"Resource/spriteData/Title.dds");
	m_sprite.SetPos(0.5f);
	m_sprite.SetScale(1.0f);
	m_sprite.SetColor({1.0f,0.0f,0.0f,1.0f});
	return true;
}

void DamegeScreenEffect::Update() {

	m_sprite.SetRot(CMath::RandomZeroToOne()*CMath::PI2);
	m_sprite.SetScale({CMath::RandomZeroToOne()*2.0f,CMath::RandomZeroToOne() * 2.0f });

	m_cnt += GetDeltaTimeSec();
	if (m_cnt > 0.125f) {
		DeleteGO(this);
		return;
	}
}
