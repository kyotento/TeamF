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
	Fall();			//���������B
	Rotation();		//��]�����B
	Distance();		//�A�C�e���擾�����B

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
	//���C�g�����f���B
	m_raytraceModel.Init(*m_model);

	//�����蔻��B
	m_colScale = CVector3::One() * Block::WIDTH * 0.25f;	//�X�P�[�����u���b�N��1/4�ɁB
	m_colPos = m_position * Block::WIDTH;					//���W���u���b�N�P�ʂɏC���B
	m_collision.CreateBox( m_colPos, CQuaternion::Identity(), m_colScale );

	// TODO: �A�C�e�����o�����ɒ��˂�����BFall()�Ƃ��킹�Ē������Ă��������B(�����_���ȕ����ɒ��˂�Ƃ�)
	m_velocity = CVector3( 0, 300, 0 );
}

//���������B
void DropItem::Fall()
{
	m_colPos = m_collision.Execute( m_velocity );
	m_velocity.y -= 15;
}

//��]�����B
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

//todo�@m_world��GetPlayer()������̂ŁA������g���ċ�����������A�v���C���[�ɃA�C�e�����擾�����鏈����ǉ����ĂˁB
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