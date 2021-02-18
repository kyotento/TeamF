#include "stdafx.h"
#include "DropItem.h"
#include "World.h"
#include "Player.h"

//DropItem::DropItem() : Entity(enEntity_None, true) {}

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

	//インスタンス数
	constexpr int ITEM_INS_NUM = 128;
	constexpr int BLOCK_INS_NUM = 256;

	//モデル。
	if (m_itemStack->GetItem().GetModelPath() == "Resource/modelData/2DFound.tkm") {
		if (m_model.Init(ITEM_INS_NUM, m_itemStack->GetItem().GetModelPath().c_str(), m_itemStack->GetItem().GetItemName())) {
			//新規ロードなら設定
			m_model.GetInstancingModel()->GetModelRender().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mts) {
				mts->SetAlbedoTexture(m_itemStack->GetItem().GetImage().GetTextueData());
			});
		}
	}
	else {
		m_model.Init(BLOCK_INS_NUM, m_itemStack->GetItem().GetModelPath().c_str(), L"DropItem");
	}
	m_model.SetScale(myScale);

	//レイトレモデル。
	m_raytraceModel.Init( m_model );

	//コリジョンのスケール。
	//m_collision.CreateBox( GetPos(), CQuaternion::Identity(), myScale * Block::WIDTH );
	m_charaCon.Init(myScale.x * Block::WIDTH * 0.5f, 0.0f, GetPos());
	//m_charaCon.SetIsDrawCollider(true);

	//アイテムが出現時に跳ねる方向。
	m_velocity += CVector3( 0, 300, 0 );

	return true;
}

void DropItem::Update()
{
	m_tick ++;
	if (m_tick >= MAX_TICK_F) {
		m_tick = 0;
		m_enableCon = true;
	}

	if (m_enableCon) {
		//落ちる処理。
		m_charaCon.Execute(m_velocity);
		if (m_charaCon.IsOnGround()) {
			m_enableCon = false;
			m_velocity = CVector3::Zero();
		}
		//m_collision.Execute( m_velocity );
		m_velocity.y -= 15;
	}

	//奈落死。
	if (GetPos().y < 0.f) {
		DeleteGO(this); 
		return;
	}

	//回る処理。
	constexpr float rotSpeed = 45;//回るスピード。度/秒。
	CQuaternion rot = m_model.GetRot();
	rot.Multiply(CQuaternion(CVector3::AxisY(), CMath::DegToRad(rotSpeed * GetDeltaTimeSec())));

	//モデルに適用。
	m_model.SetPos(GetPos());
	m_model.SetRot(rot);

	//アイテム取得処理。
	Player* player = m_world->GetPlayer();
	//プレイヤー死んでたらアイテムを取得させない。
	if (player->GetIsDeath() || m_timer <= 0.5f || m_tick%15 != 0)
	{
		m_timer += GetDeltaTimeSec();
		return;
	}

	CVector3 playerPos = player->GetPos();
	playerPos.y += 30.0f;
	//playerPos.y += 40.0;
	CVector3 diff = playerPos - GetPos();

	//この範囲内に入ったら取得するという距離。
	constexpr float catchLength = Block::WIDTH * 2.0f;

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