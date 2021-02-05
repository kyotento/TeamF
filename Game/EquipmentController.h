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
		class EquipmentController : public InventoryController {
		public:
			EquipmentController(Inventory& inventory, Inventory& craftingSlots,
				std::unique_ptr<ItemStack>& grabed);
			~EquipmentController();

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot(Event::ClickEvent& event, unsigned slotNo);

		private:
			Inventory& m_playerInventory;
		};

	}
}
