#include "stdafx.h"
#include "CraftingInventory.h"
#include "CraftingController.h"
#include "InventorySlot.h"
#include "Player.h"

namespace GUI{

	CraftingInventory::CraftingInventory( Player* player ) :
		InventoryView( player, L"Resource/spriteData/CraftingBox.dds" ), m_craftingSlots( 10 ){

		//コントローラー
		m_controller = std::make_unique<Controller::CraftingController>(
			player->GetInventory(), m_craftingSlots, m_grabed
			);

		//上部分のスロットを追加。
		const CVector2 craftingSlotPoint( 58, 37 );
		AddChilde(
			std::make_unique<InventorySlots>( m_craftingSlots, *m_controller, 0, craftingSlotPoint, 3, 3 )
		);
		const CVector2 resultSlotPoint( 246, 73 );
		AddChilde(
			std::make_unique<InventorySlots>( m_craftingSlots, *m_controller, 9, resultSlotPoint, 1, 1 )
		);
	}

	CraftingInventory::~CraftingInventory(){}

}