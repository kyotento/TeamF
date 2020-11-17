#pragma once

class Inventory;

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

			InventoryController(Inventory& inventory);

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			Inventory& m_inventory;
		};

	}

}