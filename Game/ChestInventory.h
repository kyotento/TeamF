#pragma once
//! @file
#pragma once
#include "InventoryView.h"
#include "Inventory.h"


namespace GUI {

	//! @brief Chest��Inventory��\���N���X�B
	//! @author �킵(�V�˔������B���p�t)
	class ChestInventory : public InventoryView {
	public:
		ChestInventory( Player* player, Inventory& chestInventory);

		~ChestInventory();
	private:
		//�`�F�X�g�p��27�}�X�B
		Inventory& m_inventorySlots;

		std::unique_ptr<Controller::InventoryController> m_inventoryController;
	};

}
