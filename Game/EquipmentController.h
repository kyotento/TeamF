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
		class EquipmentController : public InventoryController {
		public:
			EquipmentController(Inventory& inventory, Inventory& craftingSlots,
				std::unique_ptr<ItemStack>& grabed);
			~EquipmentController();

			//! @brief インベントリースロットをクリックしたときに呼ばれる関数。
			void OnClickSlot(Event::ClickEvent& event, unsigned slotNo);


			bool DetermineItemPlaced(int slots,const int itemId);
		private:
			Inventory& m_playerInventory;
		};

	}
}
