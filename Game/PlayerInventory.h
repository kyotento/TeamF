//! @file
#pragma once
#include "InventoryView.h"
#include "Inventory.h"


namespace GUI{

	//! @brief PlayerのInventoryを表すクラス。
	//! @author Takayama
	class PlayerInventory : public InventoryView{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();
	private:
		//クラフト用の2×2マスと成果物の1マスと装備スロットの4マス。
		Inventory m_craftingSlots;

		std::unique_ptr<Controller::InventoryController> m_controller;
		std::unique_ptr<Controller::InventoryController> m_equipmentController;
	};

}