#pragma once
#include "InventoryController.h"
#include "Furnace.h"

class Inventory;
class ItemStack;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief �N���t�g�C���x���g���̑�����s���N���X�B
		//! @author Takayama
		class FurnaceController : public InventoryController{
		public:
			FurnaceController( Inventory& furnaceSlot,
							   std::unique_ptr<ItemStack>& grabed, FurnaceFire& fire);

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			FurnaceFire& m_fire;
		};

	}
}