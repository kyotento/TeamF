#include "stdafx.h"
#include "CraftingController.h"
#include "ClickEvent.h"
#include "Inventory.h"

namespace GUI::Controller{
	CraftingController::CraftingController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed ) :
		m_playerInventory( inventory ), m_craftingSlots(craftingSlots), m_grabed( grabed ){

	}

	CraftingController::~CraftingController(){
		for( auto& item : m_craftingSlots ){
			m_playerInventory.AddItem( item );
		}
	}

	void CraftingController::OnClickSlot( Event::ClickEvent & event, unsigned slotNo ){
		using ClickType = Event::ClickEvent::ClickType;

		switch( event.GetClickType() ){
		case ClickType::LEFT:
			m_craftingSlots.LClickSlot( slotNo, m_grabed );
			break;
		case ClickType::RIGHT:
			m_craftingSlots.RClickSlot( slotNo, m_grabed );
			break;
		}
	}
}