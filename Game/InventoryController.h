#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief �C���x���g���̑�����s���N���X�B
		//! @details �N���t�g�Ƃ�����ȑ��삪�K�v�Ȃ炱�̃N���X����h��������B
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);
			virtual ~InventoryController(){}

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			virtual void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		protected:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;
		};

	}

}