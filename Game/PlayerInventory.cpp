#include "stdafx.h"
#include "PlayerInventory.h"
#include "CraftingController2.h"
#include "InventorySlot.h"
#include "EquipmentController.h"
#include "Player.h"

namespace GUI{

	PlayerInventory::PlayerInventory( Player* player ) :
		InventoryView(player, L"Resource/spriteData/KariInventory.dds", CVector4::Red()), m_craftingSlots(9) {

		//コントローラー
		m_controller = std::make_unique<Controller::CraftingController2>(
			player->GetInventory(), m_craftingSlots, m_grabed
			);

		m_equipmentController = std::make_unique<Controller::EquipmentController>(
			player->GetInventory(), m_craftingSlots, m_grabed
			);
		
		//上部分のスロットを追加。
		const CVector2 craftingSlotPoint(194, 34);
		AddChilde(
			std::make_unique<InventorySlots>(m_craftingSlots, *m_controller, 0, craftingSlotPoint, 2, 2)
		);
		const CVector2 resultSlotPoint(306, 54);
		AddChilde(
			std::make_unique<InventorySlots>(m_craftingSlots, *m_controller, 4, resultSlotPoint, 1, 1)
		);

		const CVector2 equipmentSlot(14, 14);
		AddChilde(
			std::make_unique<InventorySlots>(m_craftingSlots, *m_equipmentController, 5, equipmentSlot, 1, 4)
		);

	}

	PlayerInventory::~PlayerInventory(){}
}
