#include "stdafx.h"
#include "CraftingController.h"
#include "ClickEvent.h"
#include "Inventory.h"

namespace GUI::Controller{
	CraftingController::CraftingController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed ) :
		InventoryController(craftingSlots, grabed), m_playerInventory( inventory ){

	}

	CraftingController::~CraftingController(){
		//GUIを閉じたときにクラフトテーブルのアイテムを返却する。
		for( auto& item : m_inventory ){
			m_playerInventory.AddItem( item );
		}
	}

	void CraftingController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		InventoryController::OnClickSlot( event, slotNo );
	}
}