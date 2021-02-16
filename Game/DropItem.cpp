#include "stdafx.h"
#include "DropItem.h"
#include "World.h";
#include "Player.h";
#include "ItemStack.h"

DropItem::DropItem() : Entity(enEntity_None, true) {}

DropItem * DropItem::CreateDropItem( World * world, std::unique_ptr<ItemStack>&& item ){
	DropItem* drop = world->CreateEntity<DropItem>();
	drop->SetItemStack(std::move(item));
	return drop;
}

DropItem * DropItem::CreateDropItem( World* world, int itemID, int number){
	DropItem* drop = world->CreateEntity<DropItem>();
	drop->SetItemStack( std::make_unique<ItemStack>( Item::GetItem( itemID ), number ) );
	return drop;
}

void DropItem::SetItemStack( std::unique_ptr<ItemStack>&& item ){
	m_itemStack = std::move( item );
}

bool DropItem::Start()
{
	//SuicideObj::CSE* se;
	//se = NewGO<SuicideObj::CSE>(L"Resource/soundData/block/blockdestroy.wav");
	//se->SetVolume(1.0f);
	//se->Play();
	//スケールはブロックの4分の1。
	const CVector3 myScale = CVector3::One() * 0.25f;

	//モデル。
	m_model.Init( m_itemStack->GetItem().GetModelPath().c_str() );		//モデルの初期化。
	m_model.SetScale( myScale );

	if (m_itemStack->GetItem().GetModelPath() == "Resource/modelData/2DFound.tkm") {
		m_model.GetSkinModel().InitMaterialSetting();
		m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mts) {
			mts->SetAlbedoTexture(m_itemStack->GetItem().GetImage().GetTextueData());
		});
	}


	//レイトレモデル。
	m_raytraceModel.Init( m_model );

	//コリジョンのスケール。
	m_collision.CreateBox( GetPos(), CQuaternion::Identity(), myScale * Block::WIDTH );

	//アイテムが出現時に跳ねる方向。
	m_velocity += CVector3( 0, 300, 0 );

	return true;
}

void DropItem::Update()
{
	//落ちる処理。
	m_collision.Execute( m_velocity );
	m_velocity.y -= 15;
	m_timer += GetDeltaTimeSec();

	//回る処理。
	const float rotSpeed = 45;//回るスピード。度/秒。
	CQuaternion addRot;
	addRot.SetRotationDeg( CVector3::AxisY(), rotSpeed * GetDeltaTimeSec() );
	m_rot.Multiply( addRot );

	//モデルに適用。
	m_model.SetPos( GetPos() );
	m_model.SetRot( m_rot );

	//アイテム取得処理。
	Player* player = m_world->GetPlayer();
	//プレイヤー死んでたらアイテムを取得させない。
	if (player->GetIsDeath() || m_timer <= 0.5f)
	{
		return;
	}

	CVector3 playerPos = player->GetPos();
	playerPos.y += 30.0f;
	//playerPos.y += 40.0;
	CVector3 diff = playerPos - GetPos();

	//この範囲内に入ったら取得するという距離。
	const float catchLength = Block::WIDTH * 1.5f;

	if( diff.LengthSq() < catchLength * catchLength ){
		
		player->GetInventory().AddItem( m_itemStack );

		if( m_itemStack == nullptr ){
			SuicideObj::CSE* se;
			se = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/get.wav");
			se->SetVolume(0.3f);
			se->Play();
			DeleteGO( this );
		}
	}
}