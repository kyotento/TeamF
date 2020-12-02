#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief インベントリの操作を行うクラス。
		//! @details InventoryGUI にセットして使う。
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;
		};

	}

}