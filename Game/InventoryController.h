#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief インベントリの操作を行うクラス。
		//! @details クラフトとか特殊な操作が必要ならこのクラスから派生させる。
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);
			virtual ~InventoryController(){}

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			virtual void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		protected:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;
		};

	}

}