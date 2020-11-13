#include "stdafx.h"
#include "InventoryGUI.h"
#include "Inventory.h"
#include "ItemImage.h"
#include "ItemStack.h"
#include "ClickEvent.h"
#include "GUIManager.h"
#include "InventoryController.h"

namespace GUI{
	InventoryGUI::InventoryGUI( Inventory& inventory):
		m_inventory(inventory){
		//画像をロード。
		m_sprite.Init( L"Resource/spriteData/KariInventory.dds" );

		//位置、ピボット、スケールを設定。
		auto& grEn = GetGraphicsEngine();
		const float bufW = grEn.GetFrameBuffer_W();
		const float bufH = grEn.GetFrameBuffer_H();

		SetPos( { bufW * 0.5f, bufH * 0.5f } );
		SetPivot( { 0.5f, 0.5f } );
		SetScale( {1.5f, 1.5f} );

		//コントローラーを作成。
		m_controller = std::make_unique<Controller::InventoryController>(inventory);

		//スロットを充填。
		const CVector2 slotStart{ 32, 184 };
		for( int x = 0; x < 9; x++ ){
			for( int y = 0; y < 3; y++ ){

				CVector2 pos = slotStart;
				pos.x += x * InventorySlot::WIDTH;
				pos.y += y * InventorySlot::WIDTH;

				AddChilde( std::make_unique<InventorySlot>(inventory, *m_controller.get() ,x + y*9, pos ) );

			}
		}

		GUIManager::Instance().AddRoot( this );
	}

	InventoryGUI::~InventoryGUI(){
		GUIManager::Instance().RemoveRoot( this );
	}

	void InventoryGUI::Draw( const CVector2& pos, const CVector2& scale ){
		m_sprite.DrawScreenPos( pos, scale, GetPivot() );
	}

	void InventoryGUI::DrawForeground(){
		ItemStack* grab = m_inventory.GetGrabItem();
		if( grab ){
			//マウス座標の作成。
			CVector2 mouse = MouseCursor().GetMouseCursorPos();
			auto& grEn = GetGraphicsEngine();
			mouse.x *= grEn.GetFrameBuffer_W();
			mouse.y *= grEn.GetFrameBuffer_H();
			grab->GetItem().Draw( mouse, GetScale() );
		}
	}

	CVector2 InventoryGUI::GetSize() const{
		CVector2 size{ static_cast<float>(m_sprite.GetWidth()),
						static_cast<float>(m_sprite.GetHeight()) };
		const CVector2 scale = GetScale();

		size.x *= scale.x;
		size.y *= scale.y;

		return size;
	}

	InventorySlot::InventorySlot( Inventory& inventory, Controller::InventoryController& controller,
								  unsigned slotNo, const CVector2 & pos ):
		m_slotNo(slotNo), m_inventory( inventory ), m_controller(controller){
		SetPos( pos );
		SetPivot( { 0.5f, 0.5f } );
	}

	InventorySlot::~InventorySlot(){}

	void InventorySlot::OnClick( GUI::Event::ClickEvent & event ){
		m_controller.OnClickSlot( event, m_slotNo );
	}

	void InventorySlot::Draw( const CVector2 & pos, const CVector2 & scale ){
		ItemStack* itemStack = m_inventory.GetItem( m_slotNo );
		if( itemStack ){
			itemStack->GetItem().Draw( pos, scale );
		}
	}
}
