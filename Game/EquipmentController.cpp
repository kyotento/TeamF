#include "stdafx.h"
#include "EquipmentController.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace GUI::Controller {
	EquipmentController::EquipmentController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

	}

	EquipmentController::~EquipmentController() {
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		for (auto& item : m_inventory) {
			m_playerInventory.AddItem(item);
		}
	}

	void EquipmentController::OnClickSlot(Event::ClickEvent & event, unsigned slotNo) {
		const unsigned craftSlot = 4;

		//�N���t�g�X���b�g����Ȃ瑀�삷��B
		if (slotNo > craftSlot) {
			//�����X���b�g���N���b�N�����Ƃ��̓���B
			auto& result = m_inventory.GetItem(slotNo);
			//�����i�����ɑ��݂��Ă�����B
			if (result)
			{
				//�J�[�\���������A�C�e���������Ă��Ȃ�������B
				if (m_grabed = nullptr)
				{
					//�J�[�\���ɃA�C�e������������B
					m_grabed = std::make_unique<ItemStack>(result->GetItem(), result->GetNumber());
					//�X���b�g����ɂ���B
					result.reset();
				}
				else {
					//�}�E�X�J�[�\���ƃX���b�g�̃A�C�e�������ւ���B
					m_grabed.swap(result);
				}
			}
			//�����i�����݂��Ă��Ȃ�������B
			else 
			{
				//�J�[�\���������A�C�e���������Ă�����B
				if (m_grabed != nullptr) {
					//�C���x���g���[��1�����A�C�e����ǉ�����B
					auto& slot = m_inventory.GetItem(slotNo);
					slot = std::make_unique<ItemStack>(m_grabed->GetItem(), 1);
					int num = m_grabed->GetNumber() - 1;
					//�J�[�\���̃A�C�e����0����Ȃ�B
					if (num > 0) {
						m_grabed->SetNumber(num);
					}
					//�J�[�\���̃A�C�e���������Ȃ�����B
					else {
						m_grabed.reset();
					}	
				}
				//�J�[�\���������A�C�e���������Ă��Ȃ�������B
				else {
					//�����������Ȃ��B
					return;
				}
			}
		}
	}
}