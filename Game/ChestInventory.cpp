#include "stdafx.h"
#include "ChestInventory.h"
#include "CraftingController2.h"
#include "InventorySlot.h"
#include "EquipmentController.h"

namespace GUI {

	ChestInventory::ChestInventory( Player* player, Inventory& chestInventory) :
		InventoryView(player, L"Resource/spriteData/Chest.dds"), m_inventorySlots(chestInventory) {

		//コントローラー
		m_inventoryController = std::make_unique<Controller::InventoryController>(
			m_inventorySlots, m_grabed
		);
		m_inventoryController->SetOthorCtrl( &GetLowerController() );
		GetLowerController().SetOthorCtrl( m_inventoryController.get() );

		//上部分のスロットを追加。
		const CVector2 craftingSlotPoint(14, 40);
		AddChilde(
			std::make_unique<InventorySlots>(m_inventorySlots, *m_inventoryController, 0, craftingSlotPoint, 9, 3)
		);

	}

	ChestInventory::~ChestInventory() {}
}
