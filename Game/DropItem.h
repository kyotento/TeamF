#pragma once
#include "Entity.h"
#include "SimpleMoveObj.h"
#include "ItemType.h"
#include "MCCharaCon.h"
#include "ItemStack.h"

//! @brief Entity としてワールドに実体化したアイテムを表すクラス。
//! @details 生成にはstatic関数のCreateDropItemを使用してください。
class DropItem : public Entity
{
public:
	DropItem() {
		m_tick = MAX_TICK_F / 3 * (CMath::RandomInt()%3);
	}

	//! @brief 引数のワールドとアイテムを使用してドロップアイテムを作成します。
	//! @param item 設定するアイテム。右辺値参照なので入れる前にstd::move()が必要です。
	static DropItem* CreateDropItem( World* world, std::unique_ptr<ItemStack>&& item );

	//! @brief 引数のワールド、アイテムID、数量を使用してドロップアイテムを作成します。
	static DropItem* CreateDropItem( World* world, int itemID, int number = 1 );

	//! @param item 設定するアイテム。このインスタンスにムーブされます。
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
	/// エンティティの有効切替時の処理
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

	std::unique_ptr<ItemStack> m_itemStack;//アイテム。

	CVector3 m_velocity = CVector3::Zero(); //速度

	//SimpleMoveObj m_collision;//当たり判定。位置もここから取得する。
	MCCharaCon m_charaCon;

	bool m_enableCon = true;

	float m_timer = 0.0f;
	int m_tick = 0;

	static constexpr int MAX_TICK_F = 60;
};

