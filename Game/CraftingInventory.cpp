#include "stdafx.h"
#include "CraftingInventory.h"
#include "CraftingController.h"
#include "InventorySlot.h"

namespace GUI{

	CraftingInventory::CraftingInventory( Inventory & inventory ) :
		InventoryView( inventory, L"Resource/spriteData/CraftingBox.dds" ), m_craftingSlot( 10 ){

		//コントローラー
		auto controller = std::make_unique<Controller::CraftingController>(
			inventory, m_craftingSlot, m_grabed
			);

		//上部分のスロットを追加。
		/*const CVector2 craftingSlotPoint( 58, 37 );
		AddChilde(
			std::make_unique<InventorySlots>( m_craftingSlot, *controller, 0, craftingSlotPoint, 3, 3 )
		);
		const CVector2 resultSlotPoint( 238, 65 );
		AddChilde(
			std::make_unique<InventorySlots>( m_craftingSlot, *controller, 9, resultSlotPoint, 1, 1 )
		);
		InventorySlots( m_craftingSlot, *controller, 9, resultSlotPoint, 1, 1 );*/
	}

	CraftingInventory::~CraftingInventory(){}

}