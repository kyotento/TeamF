//! @file
#pragma once
#include "InventoryView.h"

namespace GUI{

	//! @brief Player��Inventory��\���N���X�B
	//! @author Takayama
	class PlayerInventory : public InventoryView{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();
	};

}