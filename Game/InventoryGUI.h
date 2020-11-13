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
	class InventoryGUI : public Node{
	public:
		InventoryGUI(Inventory& inventory);

		~InventoryGUI();

		//! @brief GUIManager ���Ăԕ`��֐��B
		void Draw( const CVector2& pos, const CVector2& scale ) override;

		//! @brief �őO�ʂɕ`�悷��֐��B
		void DrawForeground() override;

		//! @return GUI�̃T�C�Y�B
		CVector2 GetSize() const override;

	private:
		CSprite m_sprite;
		Inventory& m_inventory;
		std::unique_ptr<Controller::InventoryController> m_controller;
	};

	//! @brief �C���x���g���X���b�g���Ƃ̃A�C�e���\�����s���N���X�B
	//! @details �N���b�N�����m���ăR���g���[���[�֏������񂷂��Ƃ��s���B
	//! @author Takayama
	class InventorySlot : public Node{
	public:
		InventorySlot(Inventory& inventory, Controller::InventoryController& controller,
					   unsigned slotNo , const CVector2& pos );
		~InventorySlot();

		//! @return GUI�̃T�C�Y�B
		CVector2 GetSize() const override{
			return CVector2{ float(WIDTH), float(WIDTH) } * GetScale();
		}

		//! @brief GUIManager ���ĂԃN���b�N�C�x���g�֐��B
		void OnClick( GUI::Event::ClickEvent& event ) override;

		//! @brief GUIManager ���Ăԕ`��֐��B
		void Draw( const CVector2& pos, const CVector2& scale ) override;

		//! @return �C���x���g���̃X���b�g�ԍ��B
		unsigned GetSlotNo() const{
			return m_slotNo;
		}

		//! @brief �X���b�g1�̐����`�̕��B
		static constexpr unsigned WIDTH = 36;

	private:
		const unsigned m_slotNo;
		Inventory& m_inventory;
		Controller::InventoryController& m_controller;
	};

}