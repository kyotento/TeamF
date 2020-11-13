#pragma once

class Inventory;

namespace GUI{

	namespace Event{
		class ClickEvent;
	}

	namespace Controller{

		//! @brief �C���x���g���̑�����s���N���X�B
		//! @details InventoryGUI �ɃZ�b�g���Ďg���B
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory);

			//! @brief �C���x���g���[�X���b�g���N���b�N�����Ƃ��ɌĂ΂��֐��B
			void OnClickSlot( Event::ClickEvent& event, unsigned slotNo );

		private:
			Inventory& m_inventory;
		};

	}

}