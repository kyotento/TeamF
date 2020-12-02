#include "stdafx.h"
#include "InventoryController.h"
#include "Inventory.h"
#include "ClickEvent.h"

namespace GUI::Controller{
	InventoryController::InventoryController( Inventory & inventory, std::unique_ptr<ItemStack>& grabed ):
		m_inventory(inventory), m_grabed(grabed){}

	void InventoryController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		using ClickType = Event::ClickEvent::ClickType;

		switch( event.GetClickType() ){
		case ClickType::LEFT:
			m_inventory.LClickSlot(slotNo, m_grabed);
			break;
		case ClickType::RIGHT:
			m_inventory.RClickSlot( slotNo, m_grabed);
			break;
		}
	}

}
