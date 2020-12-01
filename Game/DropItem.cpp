#include "stdafx.h"
#include "DropItem.h"
#include "Inventory.h"
#include "Player.h"
#include "BlockFactory.h"
#include "World.h"

DropItem::~DropItem(){
	m_world->RemoveEntity( this );
	DeleteGO( m_model );
}

bool DropItem::Start()
{
	return true;
}

void DropItem::Update()
{
	Fall();			//落下処理。
	Rotation();		//回転処理。
	Distance();		//アイテム取得処理。

	m_position = m_colPos;
	m_position.y -= m_colScale.y / 2;
	m_model->SetPos(m_position);
}

//アイテムドロップ。
void DropItem::Drop(World* world)
{
	//モデル。
	m_model = NewGO<GameObj::CSkinModelRender>();
	m_model->Init(BlockFactory::GetModelPath(m_state));		//モデルの初期化。
	m_model->SetScale(CVector3::One() * 0.25f);

	//当たり判定。
	m_colScale = CVector3::One() * Block::WIDTH * 0.25f;	//スケールをブロックの1/4に。
	m_colPos = m_position * Block::WIDTH;					//座標をブロック単位に修正。
	m_collision.CreateBox( m_colPos, CQuaternion::Identity(), m_colScale );

	// TODO: アイテムが出現時に跳ねる方向。Fall()とあわせて調整してください。(ランダムな方向に跳ねるとか)
	m_velocity = CVector3( 0, 1000, 0 );

	m_world = world;
	world->AddEntity( this );
}

//落下処理。
void DropItem::Fall()
{
	m_colPos = m_collision.Execute( m_velocity );
	m_velocity.y -= 10;
}

//回転処理。
void DropItem::Rotation()
{
	m_rotAmount += 1;
	m_rot.SetRotationDeg(CVector3::AxisY(), m_rotAmount);
	m_model->SetRot(m_rot);
}

void DropItem::SetPos( const CVector3 & position ){
	m_position = position;
	m_colPos = m_position * Block::WIDTH;
	m_collision.SetPos( m_colPos );
}

//todo　m_worldにGetPlayer()があるので、それを使って距離判定をし、プレイヤーにアイテムを取得させる処理を追加してね。
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