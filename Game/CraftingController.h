#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{
		//TODO ���̃N���X�͂��̃N���X����p�����ׂ�
		//! @brief �N���t�g�C���x���g���̑�����s���N���X�B
		//! @author Takayama
		class CraftingController{
		public:
			CraftingController( Inventory& inventory, Inventory& craftingSlots,
								std::unique_ptr<ItemStack>& grabed );
			~CraftingController();

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_playerInventory;

			//�N���t�g�p��3�~3�}�X�Ɛ��ʕ���1�}�X�B
			Inventory& m_craftingSlots;
		};

	}
}