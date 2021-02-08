#include "stdafx.h"
#include "ChestInventory.h"
#include "CraftingController2.h"
#include "InventorySlot.h"
#include "EquipmentController.h"

namespace GUI {

	ChestInventory::ChestInventory(Inventory& inventory, Inventory& chestInventory) :
		InventoryView(inventory, L"Resource/spriteData/Chest.dds"), m_inventorySlots(chestInventory) {

		//�R���g���[���[
		m_inventoryController = std::make_unique<Controller::InventoryController>(
			m_inventorySlots, m_grabed
			);

		//�㕔���̃X���b�g��ǉ��B
		const CVector2 craftingSlotPoint(14, 40);
		AddChilde(
			std::make_unique<InventorySlots>(m_inventorySlots, *m_inventoryController, 0, craftingSlotPoint, 9, 3)
		);

	}

	ChestInventory::~ChestInventory() {}
}
