#include "stdafx.h"
#include "PlayerInventory.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "GUIManager.h"
#include "InventoryController.h"
#include "InventorySlot.h"

namespace GUI{

	PlayerInventory::PlayerInventory( Inventory& inventory ) :
		m_inventory( inventory ){
		//画像をロード。
		m_sprite.Init( L"Resource/spriteData/KariInventory.dds" );

		//位置、ピボット、スケールを設定。
		auto& grEn = GetGraphicsEngine();
		const float bufW = grEn.GetFrameBuffer_W();
		const float bufH = grEn.GetFrameBuffer_H();

		SetPos( { bufW * 0.5f, bufH * 0.5f } );
		SetPivot( { 0.5f, 0.5f } );
		SetScale( { 1.5f, 1.5f } );

		//コントローラーを作成。
		m_controller = std::make_unique<Controller::InventoryController>( inventory );

		//スロットを充填。
		//ホットバー
		const CVector2 hotBarStart{ 14, 282 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 0,
													 hotBarStart, 9, 1 ) );
		//上段の9*3のアイテム入れるところ。
		const CVector2 upperStart{ 14, 166 };
		AddChilde( std::make_unique<InventorySlots>( inventory, *m_controller.get(), 9,
													 upperStart, 9, 3) );
	}

	PlayerInventory::~PlayerInventory(){}

	void PlayerInventory::Draw( const CVector2& pos, const CVector2& parentScale ){
		m_sprite.DrawScreenPos( pos, parentScale * GetScale(), GetPivot() );
	}

	void PlayerInventory::DrawForeground(){
		ItemStack* grab = m_inventory.GetGrabItem();
		if( grab ){
			//マウス座標の作成。
			CVector2 mouse = MouseCursor().GetMouseCursorPos();
			auto& grEn = GetGraphicsEngine();
			mouse.x *= grEn.GetFrameBuffer_W();
			mouse.y *= grEn.GetFrameBuffer_H();
			grab->Draw( mouse, GetScale() );
		}
	}

	CVector2 PlayerInventory::GetSize() const{
		CVector2 size{ static_cast<float>( m_sprite.GetWidth() ),
						static_cast<float>( m_sprite.GetHeight() ) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}
}
