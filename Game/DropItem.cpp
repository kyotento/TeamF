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

	Fall();			//落下処理。
	Rotation();		//回転処理。

	m_collision->SetPosition(m_colPos);
	m_position = m_colPos;
	m_position.y -= m_colScale.y / 2;
	m_model->SetPos(m_position);
}

//アイテムドロップ。
void DropItem::Drop()
{
	//モデル。
	m_model = NewGO<GameObj::CSkinModelRender>();
	m_model->Init(BlockFactory::GetModelPath(m_state));		//モデルの初期化。
	m_model->SetScale(CVector3::One() * 0.25f);

	//当たり判定。
	m_collision = std::make_unique<SuicideObj::CCollisionObj>();
	m_collision->SetIsStaticObject(true);
	m_colScale = CVector3::One() * Block::WIDTH * 0.25f;			//スケールをブロックの1/4に。
	m_colPos = m_position * Block::WIDTH;							//座標をブロック単位に修正。
	m_collision->CreateBox(m_colPos * Block::WIDTH, CQuaternion::Identity(), m_colScale);
	m_collision->SetTimer(enNoTimer);
}

//落下処理。
void DropItem::Fall()
{
	//todo 髙山君　こいつの周りにも判定付けてほしい。
	//判定をとった時に座標の更新を終わる。
	//m_colPos.y -= 1.f;
}

//回転処理。
void DropItem::Rotation()
{
	m_rotAmount += 1;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_rotAmount);
	m_model->SetRot(m_rot);
}

//todo　いらん。
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