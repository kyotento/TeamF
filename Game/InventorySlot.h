//! @file
#pragma once
#include "Node.h"

class Inventory;

namespace GUI{

	namespace Controller{
		class InventoryController;
	}

	//! @brief �C���x���g���X���b�g���Ƃ̃A�C�e���\�����s���N���X�B
		//! @details �N���b�N�����m���ăR���g���[���[�֏������񂷂��Ƃ��s���B
		//! @author Takayama
	class InventorySlots : public Node{
	public:
		InventorySlots( Inventory& inventory, Controller::InventoryController& controller,
						unsigned slotNoStart, const CVector2& posUpperLeft, unsigned widthNum, unsigned heightNum );
		~InventorySlots();

		//! @return GUI�̃T�C�Y�B
		CVector2 GetSize() const override{
			return CVector2{ float( m_widthNum * SLOT_WIDTH ), float( m_heightNum * SLOT_WIDTH ) } *GetScale();
		}

		//! @brief GUIManager ���Ăԃ}�E�X����������C�x���g�֐��B
		void OnMouseEvent( GUI::Event::MouseEvent& event ) override;

		//! @brief GUIManager ���Ăԕ`��֐��B
		void Draw( const CVector2& pos, const CVector2& parentScale ) override;

		//! @brief �X���b�g1�̐����`�̕��B
		static constexpr unsigned SLOT_WIDTH = 36;

	private:
		const unsigned m_slotNoStart;
		const unsigned m_widthNum;
		const unsigned m_heightNum;

		Inventory& m_inventory;
		Controller::InventoryController& m_controller;
	};

}