#pragma once
#include "InventoryView.h"
#include "Inventory.h"

namespace GUI{

	class CraftingInventory : public InventoryView{
	public:
		CraftingInventory( Player* player );

		~CraftingInventory();

	private:
		//クラフト用の3×3マスと成果物の1マス。
		Inventory m_craftingSlots;

		std::unique_ptr<Controller::InventoryController> m_controller;
	};

}