#pragma once
#include "Node.h"

class Inventory;
class ItemStack;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief Inventory�nGUI�̊��N���X
	//! @author Takayama
	class InventoryView : public RootNode{
	public:
		InventoryView( Inventory& playerInventory, const wchar_t* spritePath, const CVector4& m_shiftColor = CVector4::One());

		virtual ~InventoryView();

		//! @brief GUIManager ���Ăԕ`��֐��B
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief �őO�ʂɕ`�悷��֐��B
		void DrawForeground() override;

		//! @return GUI�̃T�C�Y�B
		CVector2 GetSize() const override;

	protected:
		//�͂�ł���A�C�e��
		std::unique_ptr<ItemStack> m_grabed;
	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
		CVector4 m_shiftColor = CVector4::One();
	};

}