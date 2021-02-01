#include "stdafx.h"
#include "DropItem.h"
#include "World.h";
#include "Player.h";
#include "ItemStack.h"

DropItem * DropItem::CreateDropItem( World * world, std::unique_ptr<ItemStack>&& item ){
	DropItem* drop = world->CreateEntity<DropItem>();
	drop->SetItemStack( std::move(item) );
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
	//�X�P�[���̓u���b�N��4����1�B
	const CVector3 myScale = CVector3::One() * 0.25f;

	//���f���B
	m_model.Init( m_itemStack->GetItem().GetModelPath().c_str() );		//���f���̏������B
	m_model.SetScale( myScale );
	//���C�g�����f���B
	m_raytraceModel.Init( m_model );

	//�R���W�����̃X�P�[���B
	m_collision.CreateBox( GetPos(), CQuaternion::Identity(), myScale * Block::WIDTH );

	//�A�C�e�����o�����ɒ��˂�����B
	m_velocity += CVector3( 0, 300, 0 );

	return true;
}

void DropItem::Update()
{
	//�����鏈���B
	m_collision.Execute( m_velocity );
	m_velocity.y -= 15;

	//��鏈���B
	const float rotSpeed = 45;//���X�s�[�h�B�x/�b�B
	CQuaternion addRot;
	addRot.SetRotationDeg( CVector3::AxisY(), rotSpeed * GetDeltaTimeSec() );
	m_rot.Multiply( addRot );

	//���f���ɓK�p�B
	m_model.SetPos( GetPos() );
	m_model.SetRot( m_rot );

	//�A�C�e���擾�����B
	Player* player = m_world->GetPlayer();
	if (player->GetIsDeath())
	{
		return;
	}

	CVector3 diff = player->GetPos() - GetPos();

	//���͈͓̔��ɓ�������擾����Ƃ��������B
	const float catchLength = Block::WIDTH;

	if( diff.LengthSq() < catchLength * catchLength ){

		player->GetInventory().AddItem( m_itemStack );

		if( m_itemStack == nullptr ){
			DeleteGO( this );
		}
	}
}