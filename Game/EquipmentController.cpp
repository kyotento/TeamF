#include "stdafx.h"
#include "EquipmentController.h"
#include "MouseEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"

namespace {
	const unsigned craftSlot = 4;
	const unsigned playerSlot = 35;
	const unsigned equipSlot = 5;
	int equipSlotList[4] = { enTool_Helmet,enTool_Plate,enTool_Leggings,enTool_Boots };
}

namespace GUI::Controller {
	EquipmentController::EquipmentController(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

		//�C���x���g���ɃA�C�e�����[�U����B
		for (int i = 1; i < equipSlot; i++)
		{
			auto& pSlot = m_playerInventory.GetItem(i + playerSlot);
			if (m_playerInventory.GetNullableItem(i + playerSlot).GetID() != enCube_None
				&& pSlot) {
				auto item = std::make_unique<ItemStack>(Item::GetItem(pSlot.get()->GetID()), 1);
				m_inventory.SetItem(i+craftSlot,std::move(item));
			}
		}
	}

	EquipmentController::~EquipmentController() {
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		/*for (auto& item : m_inventory) {
			m_playerInventory.AddItem(item);
		}*/
	}

	void EquipmentController::OnMouseEvent(Event::MouseEvent & event, unsigned slotNo) {
		if( event.IsClick() == false ){
			return;
		}

		//�N���t�g�X���b�g����Ȃ瑀�삷��B
		if (slotNo > craftSlot) {
			//�����X���b�g���N���b�N�����Ƃ��̓���B
			//auto& result = m_playerInventory.GetItem(slotNo + playerSlot);
			auto& result = m_inventory.GetItem(slotNo);
			//�X���b�g�ɑ����i�����ɑ��݂��Ă�����B
			if (result != nullptr)
			{
				//�J�[�\���������A�C�e���������Ă��Ȃ�������B
				if (m_grabed == nullptr)
				{
					//�J�[�\���ɃA�C�e������������B
					m_grabed = std::make_unique<ItemStack>(result->GetItem(), result->GetNumber());
					//�X���b�g����ɂ���B
					result.reset();
					//�v���C���[�C���x���g������ɂ���B
					m_playerInventory.GetItem(slotNo + playerSlot- craftSlot).reset();
					return;
				}
				//�J�[�\�����A�C�e���������Ă�����B
				else {
					//�X���b�g�ƃ}�E�X�J�[�\���̃A�C�e��������Ă�����B
					if (m_grabed->GetID() != result->GetID())
					{
						if (!DetermineItemPlaced(slotNo, m_grabed->GetToolID()))
						{
							return;
						}
						//�}�E�X�J�[�\���̃A�C�e�����������������B
						if (m_grabed->GetNumber() == 1)
						{

							//�}�E�X�J�[�\���ƃX���b�g�̃A�C�e�������ւ���B
							m_grabed.swap(result);
							auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
							//�v���C���[�X���b�g�̃A�C�e�����ς���B
							//pSlot = std::make_unique<ItemStack>(result->GetItem());
							pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);

							
						}
						//TODO �����Ȃ������Ăǂ������ł����ˁB���������������đ��̃A�C�e���݂����ɕ�����C�Ɏ��ĂȂ��Ǝv����ł�����(������)�B
						else {

						}
						return;
					}
					//�X���b�g�ƃ}�E�X�J�[�\���̃A�C�e�����ꏏ��������B
					else {
						//������ނ̃A�C�e���������Ă���B
						int tempNum = result->GetNumber() + m_grabed->GetNumber();
						const int stackLimit = result->GetStackLimit();
						auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
						//�A�C�e����ꂽ��B
						if (tempNum > stackLimit) {
							m_grabed->SetNumber(stackLimit);
							result->SetNumber(tempNum - stackLimit);
							//�v���C���[�X���b�g�̃A�C�e�����ς���B
							pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);
						}
						else {
							//���Ȃ�������B
							m_grabed->SetNumber(tempNum);
							result.reset();
							pSlot.reset();
						}
						return;
					}
				}
			}
			//�X���b�g�ɑ����i�����݂��Ă��Ȃ�������B
			else 
			{
				//�J�[�\���������A�C�e���������Ă�����B
				if (m_grabed != nullptr) {
					//�J�[�\�����Y���̃A�C�e������Ȃ������牽�����Ȃ��B
					if (!DetermineItemPlaced(slotNo, m_grabed->GetToolID()))
					{
						return;
					}
					//�C���x���g���[��1�����A�C�e����ǉ�����B
					auto& slot = m_inventory.GetItem(slotNo);
			
					slot = std::make_unique<ItemStack>(m_grabed->GetItem(), 1);
					auto& pSlot = m_playerInventory.GetItem(slotNo + playerSlot- craftSlot);
					//�v���C���[�X���b�g�̃A�C�e�����ς���B
					pSlot = std::make_unique<ItemStack>(Item::GetItem(result.get()->GetID()), 1);
					int num = m_grabed->GetNumber() - 1;
					//�J�[�\���̃A�C�e����0����Ȃ�B
					if (num > 0) {
						m_grabed->SetNumber(num);
					}
					//�J�[�\���̃A�C�e���������Ȃ�����B
					else {
						m_grabed.reset();
					}
					return;
				}
				//�J�[�\���������A�C�e���������Ă��Ȃ�������B
				else {
					//�����������Ȃ��B
					return;
				}
			}
		}
	}

	bool EquipmentController::DetermineItemPlaced(int slots,const int itemId)
	{
		//��������
		if (GetKeyInput(VK_TAB)) {
			return true;
		}

		slots = slots - craftSlot;
		if (itemId == equipSlotList[slots-1])
		{
			return true;
		}
		else {
			return false;
		}
	}
}

