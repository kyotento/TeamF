#include "stdafx.h"
#include "DropItem.h"
#include "Inventory.h"
#include "Player.h"



bool DropItem::Start()
{
	m_model = NewGO<GameObj::CSkinModelRender>();
	m_model->Init(L"Resource/modelData/GrassBlock.tkm");
	m_model->SetScale(CVector3::One() * 0.001f);
	return true;
}

void DropItem::Update()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	m_model->SetPos(m_position);
	Distance();
}

void DropItem::Distance()
{
	/*if (m_box == nullptr) {
		m_box = FindGO<Box>();
		return;
	}

	const float distance = 2.0f * 2.0f;

	CVector3 diff = m_player->GetPos() - m_position;
	if (diff.LengthSq() <= distance) {
		int number = m_box->AddItem(m_state, m_number);
		if (number == 0) {
			DeleteGO(this);
		}
		else {
			m_number = number;
		}
	}*/
}