#include "stdafx.h"
#include "FurnaceInventory.h"
#include "InventorySlot.h"
#include "Furnace.h"
#include "FurnaceController.h"

namespace GUI{

	FurnaceInventory::FurnaceInventory( Player* player, Inventory& furnaceInv, FurnaceFire& fire ) :
		InventoryView( player, L"Resource/spriteData/FurnaceBox.dds" ){

		//コントローラー
		m_controller = std::make_unique<Controller::FurnaceController>(
			furnaceInv, m_grabed, fire
		);

		//上部分のスロットを追加。
		const CVector2 fuelPos( 110, 104 );
		AddChilde(
			std::make_unique<InventorySlots>( furnaceInv, *m_controller, Furnace::FUEL, fuelPos, 1, 1 )
		);
		const CVector2 materialPos( 110, 32 );
		AddChilde(
			std::make_unique<InventorySlots>( furnaceInv, *m_controller, Furnace::SOURCE, materialPos, 1, 1 )
		);
		const CVector2 resultPos( 230, 68 );
		AddChilde(
			std::make_unique<InventorySlots>( furnaceInv, *m_controller, Furnace::RESULT, resultPos, 1, 1 )
		);

		//火と矢印の画像
		const CVector2 firePos( 112, 72 );
		AddChilde(
			std::make_unique<FurnaceFireImg>(fire, firePos)
		);
		const CVector2 arrowPos( 160, 68 );
		AddChilde(
			std::make_unique<FurnaceArrowImg>(fire, arrowPos)
		);
	}

	FurnaceInventory::~FurnaceInventory(){}

	FurnaceFireImg::FurnaceFireImg( FurnaceFire& fire, const CVector2& pos ): m_fire(fire){
		SetPos( pos );
		m_sprite.Init( L"Resource/spriteData/FurnaceFire.dds" );
	}

	void FurnaceFireImg::Draw( const CVector2 & pos, const CVector2 & parentScale ){
		m_sprite.SetSourceRectangle(0, 1 - m_fire.GetFirePer(), 1, 1 );
		CVector2 pos2 = pos;
		pos2.y += parentScale.y * GetSize().y * (1 - m_fire.GetFirePer());
		m_sprite.DrawScreenPos( pos2, parentScale * GetScale(), GetPivot() );
	}

	CVector2 FurnaceFireImg::GetSize() const{
		CVector2 size{ static_cast<float>( m_sprite.GetWidth() ),
						static_cast<float>( m_sprite.GetHeight() ) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

	FurnaceArrowImg::FurnaceArrowImg( FurnaceFire& fire, const CVector2& pos ): m_fire(fire){
		SetPos( pos );
		m_sprite.Init( L"Resource/spriteData/FurnaceArrow.dds" );
	}

	void FurnaceArrowImg::Draw( const CVector2 & pos, const CVector2 & parentScale ){
		m_sprite.SetSourceRectangle( 0, 0, m_fire.GetArrowPer(), 1 );
		m_sprite.DrawScreenPos( pos, parentScale * GetScale(), GetPivot() );
	}

	CVector2 FurnaceArrowImg::GetSize() const{
		CVector2 size{ static_cast<float>( m_sprite.GetWidth() ),
						static_cast<float>( m_sprite.GetHeight() ) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

}