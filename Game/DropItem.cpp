#include "stdafx.h"
#include "DropItem.h"
#include "Inventory.h"
#include "Player.h"
#include "BlockFactory.h"

bool DropItem::Start()
{
	return true;
}

void DropItem::Update()
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}

	Fall();		//���������B

	m_collision->SetPosition(m_colPos);
	m_position = m_colPos;
	m_position.y -= m_colScale.y / 2;
	m_model->SetPos(m_position);
}

//�A�C�e���h���b�v�B
void DropItem::Drop()
{
	//���f���B
	m_model = NewGO<GameObj::CSkinModelRender>();
	m_model->Init(BlockFactory::GetModelPath(m_state));		//���f���̏������B
	m_model->SetScale(CVector3::One() * 0.25f);
	//�����蔻��B
	m_collision = std::make_unique<SuicideObj::CCollisionObj>();
	m_collision->SetIsStaticObject(true);
	m_colScale = CVector3::One() * Block::WIDTH * 0.25f;			//�X�P�[�����u���b�N��1/4�ɁB
	m_colPos = m_position * Block::WIDTH;							//���W���u���b�N�P�ʂɏC���B
	m_collision->CreateBox(m_colPos * Block::WIDTH, CQuaternion::Identity(), m_colScale);
	m_collision->SetTimer(enNoTimer);
}

void DropItem::Fall()
{

}

//todo�@�����B
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