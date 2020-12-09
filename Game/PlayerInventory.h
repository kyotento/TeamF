//! @file
#pragma once
#include "InventoryView.h"

namespace GUI{

	//! @brief PlayerのInventoryを表すクラス。
	//! @author Takayama
	class PlayerInventory : public InventoryView{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();
	};

}