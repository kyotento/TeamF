#pragma once

class Inventory;
class ItemStack;
class Item;

namespace GUI{

	namespace Event{
		class MouseEvent;
	}

	namespace Controller{

		//! @brief �C���x���g���̑�����s���N���X�B
		//! @details �N���t�g�Ƃ�����ȑ��삪�K�v�Ȃ炱�̃N���X����h��������B
		//! @author Takayama
		class InventoryController{
		public:

			InventoryController(Inventory& inventory, std::unique_ptr<ItemStack>& grabed);
			virtual ~InventoryController(){}

			//! @brief �C���x���g���[�X���b�g�Ƀ}�E�X���������Ƃ��ɌĂ΂��֐��B
			virtual void OnMouseEvent( Event::MouseEvent& event, unsigned slotNo );

		protected:
			//! @brief �A�C�e���擾�̓���B
			//! @return �N���b�N���s��ꂽ�Ȃ�true�B
			bool ClickGet( Event::MouseEvent& event, unsigned slotNo );

			std::unique_ptr<ItemStack>& m_grabed;
			Inventory& m_inventory;

			//�h���b�O���̃X���b�g�����B
			std::unordered_set<int> m_dragSlots;
			//�h���b�O���̃A�C�e��
			Item* m_dragItem = nullptr;

			//����X�e�[�g
			enum{
				ON_PUT,
				ON_GET,
				NO_DOWN
			};
			int m_state = NO_DOWN;
		};

	}

}