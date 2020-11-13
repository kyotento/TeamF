#include "stdafx.h"
#include "InventoryController.h"
#include "Inventory.h"
#include "ClickEvent.h"

namespace GUI::Controller{
	InventoryController::InventoryController( Inventory & inventory ) : m_inventory(inventory){}

	void InventoryController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		using ClickType = Event::ClickEvent::ClickType;

		switch( event.GetClickType() ){
		case ClickType::LEFT:
			m_inventory.LClickSlot(slotNo);
			break;
		case ClickType::RIGHT:
			m_inventory.RClickSlot( slotNo );
			break;
		}
	}

}
