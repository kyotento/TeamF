#pragma once
#include "InventoryController.h"

class Inventory;
class ItemStack;

namespace GUI {

	namespace Event {
		class ClickEvent;
	}

	namespace Controller {

		//! @brief クラフトインベントリの操作を行うクラス。
		//! @author Takayama
		class CraftingController2 : public InventoryController {
		public:
			CraftingController2(Inventory& inventory, Inventory& craftingSlots,
				std::unique_ptr<ItemStack>& grabedz);
			~CraftingController2();

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnClickSlot(Event::ClickEvent& event, unsigned slotNo);

		private:
			Inventory& m_playerInventory;
		};

	}
}