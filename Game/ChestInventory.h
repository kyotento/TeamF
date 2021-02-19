#pragma once
//! @file
#pragma once
#include "InventoryView.h"
#include "Inventory.h"


namespace GUI {

	//! @brief ChestのInventoryを表すクラス。
	//! @author わし(天才美少女錬金術師)
	class ChestInventory : public InventoryView {
	public:
		ChestInventory( Player* player, Inventory& chestInventory);

		~ChestInventory();
	private:
		//チェスト用の27マス。
		Inventory& m_inventorySlots;

		std::unique_ptr<Controller::InventoryController> m_inventoryController;
	};

}
