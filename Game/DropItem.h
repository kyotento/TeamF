#pragma once
#include "Entity.h"
#include "SimpleMoveObj.h"
#include "ItemType.h"
#include "MCCharaCon.h"
#include "ItemStack.h"

//! @brief Entity �Ƃ��ă��[���h�Ɏ��̉������A�C�e����\���N���X�B
//! @details �����ɂ�static�֐���CreateDropItem���g�p���Ă��������B
class DropItem : public Entity
{
public:
	DropItem() {
		m_tick = MAX_TICK_F / 3 * (CMath::RandomInt()%3);
	}

	//! @brief �����̃��[���h�ƃA�C�e�����g�p���ăh���b�v�A�C�e�����쐬���܂��B
	//! @param item �ݒ肷��A�C�e���B�E�Ӓl�Q�ƂȂ̂œ����O��std::move()���K�v�ł��B
	static DropItem* CreateDropItem( World* world, std::unique_ptr<ItemStack>&& item );

	//! @brief �����̃��[���h�A�A�C�e��ID�A���ʂ��g�p���ăh���b�v�A�C�e�����쐬���܂��B
	static DropItem* CreateDropItem( World* world, int itemID, int number = 1 );

	//! @param item �ݒ肷��A�C�e���B���̃C���X�^���X�Ƀ��[�u����܂��B
	void SetItemStack( std::unique_ptr<ItemStack>&& item );

	void SetPos( const CVector3& position ) override{
		m_charaCon.SetPosition(position);// m_collision.SetPos(position);
	}

	void SetVelocity( const CVector3& velocity ){
		m_velocity = velocity;
	}

	CVector3 GetPos() const override{
		return m_charaCon.GetPosition();// m_collision.GetPos();
	}

	/// <summary>
	/// �G���e�B�e�B�̗L���ؑ֎��̏���
	/// </summary>
	/// <param name="enable"></param>
	void SetEnableEntityInner(bool enable)override {
		//m_collision.GetCollision().SetEnable(enable);
		m_model.SetEnable(enable);
	}

	bool Start() override;
	void Update() override;

private:
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;

	std::unique_ptr<ItemStack> m_itemStack;//�A�C�e���B

	CVector3 m_velocity = CVector3::Zero(); //���x

	//SimpleMoveObj m_collision;//�����蔻��B�ʒu����������擾����B
	MCCharaCon m_charaCon;

	bool m_enableCon = true;

	float m_timer = 0.0f;
	int m_tick = 0;

	static constexpr int MAX_TICK_F = 60;
};

