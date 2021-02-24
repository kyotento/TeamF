#pragma once
#include "InventoryController.h"

class Inventory;
class ItemStack;

namespace GUI {

	namespace Event {
		class MouseEvent;
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
			void OnMouseEvent(Event::MouseEvent& event, unsigned slotNo);

		private:
			void ShiftCraft( std::unique_ptr<ItemStack>&& result );

			Inventory& m_playerInventory;
		};

	}
}