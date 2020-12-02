#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief �C���x���g���̑�����s���N���X�B
		//! @details InventoryGUI �ɃZ�b�g���Ďg���B
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;
		};

	}

}