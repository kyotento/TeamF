#pragma once

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{
		//TODO このクラスはあのクラスから継承すべき
		//! @brief クラフトインベントリの操作を行うクラス。
		//! @author Takayama
		class CraftingController{
		public:
			CraftingController( Inventory& inventory, Inventory& craftingSlots,
								std::unique_ptr<ItemStack>& grabed );
			~CraftingController();

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_playerInventory;

			//クラフト用の3×3マスと成果物の1マス。
			Inventory& m_craftingSlots;
		};

	}
}