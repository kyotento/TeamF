#pragma once
#include "InventoryController.h"
#include "Furnace.h"

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class MouseEvent;
	}

	namespace Controller{

		//! @brief クラフトインベントリの操作を行うクラス。
		//! @author Takayama
		class FurnaceController : public InventoryController{
		public:
			FurnaceController( Inventory& furnaceSlot,
							   std::unique_ptr<ItemStack>& grabed, FurnaceFire& fire);

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnMouseEvent( Event::MouseEvent& event, unsigned slotNo );

		private:
			FurnaceFire& m_fire;
		};

	}
}