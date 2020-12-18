#include "stdafx.h"
#include "DropItem.h"
#include "Inventory.h"
#include "Player.h"
#include "BlockFactory.h"
#include "World.h"
#include "ItemStack.h"

DropItem::DropItem(World* world) : Entity(world)
{

}

DropItem::~DropItem(){
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
void DropItem::Drop()
{
	//モデル。
	m_model = NewGO<GameObj::CSkinModelRender>();
	m_model->Init(BlockFactory::GetModelPath(m_state));		//モデルの初期化。
	m_model->SetScale(CVector3::One() * 0.25f);
	//レイトレモデル。
	m_raytraceModel.Init(*m_model);

	//当たり判定。
	m_colScale = CVector3::One() * Block::WIDTH * 0.25f;	//スケールをブロックの1/4に。
	m_colPos = m_position * Block::WIDTH;					//座標をブロック単位に修正。
	m_collision.CreateBox( m_colPos, CQuaternion::Identity(), m_colScale );

	// TODO: アイテムが出現時に跳ねる方向。Fall()とあわせて調整してください。(ランダムな方向に跳ねるとか)
	m_velocity = CVector3( 0, 300, 0 );
}

//落下処理。
void DropItem::Fall()
{
	m_colPos = m_collision.Execute( m_velocity );
	m_velocity.y -= 15;
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
	Player* player = m_world->GetPlayer();

	CVector3 diff = player->GetPos() - m_colPos;

	const float catchLength = Block::WIDTH;

	if (diff.LengthSq() < catchLength * catchLength) {

		auto item = std::make_unique<ItemStack>(Item::GetItem(m_state), 1);

		player->GetInventory().AddItem(item);

		DeleteGO(this);
	}
}