#pragma once
#include "InventoryView.h"
#include "Inventory.h"

namespace GUI{

	class CraftingInventory : public InventoryView{
	public:
		CraftingInventory( Inventory& inventory );

		~CraftingInventory();

	private:
		//�N���t�g�p��3�~3�}�X�Ɛ��ʕ���1�}�X�B
		Inventory m_craftingSlot;
	};

}