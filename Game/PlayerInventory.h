//! @file
#pragma once
#include "InventoryView.h"
#include "Inventory.h"


namespace GUI{

	//! @brief Player��Inventory��\���N���X�B
	//! @author Takayama
	class PlayerInventory : public InventoryView{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();
	private:
		//�N���t�g�p��2�~2�}�X�Ɛ��ʕ���1�}�X�B
		Inventory m_craftingSlots;
		Inventory m_equipmentSlots;

		std::unique_ptr<Controller::InventoryController> m_controller;
		std::unique_ptr<Controller::InventoryController> m_equipmentController;
	};

}