#include "stdafx.h"
#include "InventorySlot.h"
#include "Inventory.h"
#include "ItemStack.h"
#include "MouseEvent.h"
#include "GUIManager.h"
#include "InventoryController.h"

namespace GUI{

	InventorySlots::InventorySlots(
		Inventory& inventory, Controller::InventoryController& controller,
		unsigned slotNoStart, const CVector2& posUpperLeft, unsigned widthNum, unsigned heightNum ) :

		m_slotNoStart( slotNoStart ), m_inventory( inventory ), m_controller( controller ),
		m_widthNum( widthNum ), m_heightNum( heightNum ){

		SetPos( posUpperLeft );
		SetPivot( { 0, 0 } );
	}

	InventorySlots::~InventorySlots(){}

	void InventorySlots::OnMouseEvent( GUI::Event::MouseEvent & event ){
		CVector2 pos = event.GetPos();

		int x = int( std::floorf( pos.x / ( float( SLOT_WIDTH ) ) ) );
		int y = int( std::floorf( pos.y / ( float( SLOT_WIDTH ) ) ) );
		m_controller.OnMouseEvent( event, m_slotNoStart + x + y * m_widthNum );
	}

	void InventorySlots::Draw( const CVector2 & pos, const CVector2 & parentScale ){
		const int width = m_widthNum;
		const int height = m_heightNum;

		//アイテムのピボットぶんだけ開始位置をずらす。
		const CVector2 startPos = pos + ( CVector2{ 0.5f, 0.5f } *SLOT_WIDTH * parentScale );

		//自分に含まれるすべてのスロットのアイテムを描画。
		for( int y = 0; y < height; y++ ){
			for( int x = 0; x < width; x++ ){
				//列と行からスロット番号を算出。
				auto& itemStack = m_inventory.GetItem( m_slotNoStart + x + y * m_widthNum );
				if( itemStack ){
					CVector2 plusPos{ float( x * SLOT_WIDTH ) , float( y * SLOT_WIDTH ) };
					plusPos *= parentScale;
					itemStack->Draw( startPos + plusPos, parentScale * GetScale() );
				}
			}
		}
	}
}