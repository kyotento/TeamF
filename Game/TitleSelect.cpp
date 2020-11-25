#include "stdafx.h"
#include "TitleSelect.h"
#include "GUIManager.h"

TitleSelect::TitleSelect()
{
	m_spriteRneder[0].Init(L"Resource/spriteData/GameStartButton.dds");
	m_spriteRneder[1].Init(L"Resource/spriteData/GameEndButton.dds");
	m_spriteRneder[2].Init(L"Resource/spriteData/ConfigurationButton.dds");

	for (int i = 0; i < m_buttonNum; i++) {
		m_position[i] = { 0.5f,0.5f };
		float posY = 0.1f * i;
		m_position[i].y += posY;
		m_spriteRneder[i].SetPos(m_position[i]);
		m_spriteRneder[i].SetScale(m_scale);
	}

//	GUIManager::Instance().AddRoot(this);

	
}

TitleSelect::~TitleSelect()
{
}

CVector2 TitleSelect::GetSize() const 
{
	CVector2 scale = { 292.f,28.f };
	scale *= m_scale;
	return scale;
}