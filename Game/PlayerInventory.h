//! @file
#pragma once
#include "Node.h"

class Inventory;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief �C���x���g���̉�ʕ\�����s���N���X�B
	//! @author Takayama
	class PlayerInventory : public RootNode{
	public:
		PlayerInventory(Inventory& inventory);

		~PlayerInventory();

		//! @brief GUIManager ���Ăԕ`��֐��B
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief �őO�ʂɕ`�悷��֐��B
		void DrawForeground() override;

		//! @return GUI�̃T�C�Y�B
		CVector2 GetSize() const override;

	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
	};

}