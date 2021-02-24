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

		//! @brief �N���t�g�C���x���g���̑�����s���N���X�B
		//! @author Takayama
		class FurnaceController : public InventoryController{
		public:
			FurnaceController( Inventory& furnaceSlot,
							   std::unique_ptr<ItemStack>& grabed, FurnaceFire& fire);

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnMouseEvent( Event::MouseEvent& event, unsigned slotNo );

			void AddItem( std::unique_ptr<ItemStack>& item ) override;

		private:
			FurnaceFire& m_fire;
		};

	}
}