#pragma once
#include "InventoryController.h"

class Inventory;
class ItemStack;

namespace GUI {

	namespace Event {
		class ClickEvent;
	}

	namespace Controller {

		//! @brief �N���t�g�C���x���g���̑�����s���N���X�B
		//! @author Takayama
		class CraftingController2 : public InventoryController {
		public:
			CraftingController2(Inventory& inventory, Inventory& craftingSlots,
				std::unique_ptr<ItemStack>& grabedz);
			~CraftingController2();

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot(Event::ClickEvent& event, unsigned slotNo);

		private:
			Inventory& m_playerInventory;
		};

	}
}