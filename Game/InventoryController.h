#pragma once

class Inventory;
class ItemStack;
class Item;

namespace GUI{

	namespace Event{
		class MouseEvent;
	}

	namespace Controller{

		//! @brief インベントリの操作を行うクラス。
		//! @details クラフトとか特殊な操作が必要ならこのクラスから派生させる。
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);
			virtual ~InventoryController(){}

			//! @brief インベントリースロットにマウスが入ったときに呼ばれる関数。
			virtual void OnMouseEvent( Event::MouseEvent& event, unsigned slotNo );

		protected:
			//! @brief アイテム取得の動作。
			//! @return クリックが行われたならtrue。
			bool ClickGet( Event::MouseEvent& event, unsigned slotNo );

			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;

			//ドラッグ中のスロットたち。
			std::unordered_set<int> m_dragSlots;
			//ドラッグ中のアイテム
			Item* m_dragItem = nullptr;

			//操作ステート
			enum{
				ON_PUT,
				ON_GET,
				NO_DOWN
			};
			int m_state = NO_DOWN;
		};

	}

}