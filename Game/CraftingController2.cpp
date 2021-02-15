#include "stdafx.h"
#include "CraftingController2.h"
#include "ClickEvent.h"
#include "Inventory.h"
#include "RecipeManager.h"
#include "DropItem.h"
#include "Block.h"
#include "Player.h"

namespace GUI::Controller {
	CraftingController2::CraftingController2(
		Inventory & inventory, Inventory& craftingSlots, std::unique_ptr<ItemStack>& grabed) :
		InventoryController(craftingSlots, grabed), m_playerInventory(inventory) {

	}

	CraftingController2::~CraftingController2() {
		const unsigned resultSlot = 4;
		//GUI������Ƃ��ɃN���t�g�e�[�u���̃A�C�e����ԋp����B
		int number = 0;
		for (auto& item : m_inventory) 
		{	//�N���t�g�X���b�g�̂Ƃ��낾���A�C�e����ԋp����B
			if (number <= resultSlot - 1) {
				//�A�C�e�����v���C���[�C���x���g���ɕԋp�ł��Ȃ�������B
				if (m_inventory.GetNullableItem(number).GetID() != enCube_None && !m_playerInventory.AddItem(item))
				{
					//�h���b�v�A�C�e���𐶐�����B
					auto player = FindGO<Player>();
					if (player != nullptr)
					{
						player->CreateFrontDropItem(item);
					}

				}
			}
			number++;
		}
	}

	void CraftingController2::OnClickSlot(Event::ClickEvent & event, unsigned slotNo) {
		const unsigned resultSlot = 4;

		if (slotNo == resultSlot) {
			//�����i�X���b�g���N���b�N�����Ƃ��̓���B
			auto& result = m_inventory.GetItem(resultSlot);

			if (result == nullptr) {
				return;
			}
			if (m_grabed == nullptr) {
				//�J�[�\������Ȃ��0�̊����i�Ɠ����A�C�e������������B
				m_grabed = std::make_unique<ItemStack>(result->GetItem(), 0);
			}
			if (m_grabed->GetID() != result->GetID()) {
				return;
			}

			//�A�C�e���ړ��̏���
			const int grabNum = m_grabed->GetNumber();
			const int resultNum = result->GetNumber();

			//�J�[�\���̃A�C�e�����X�^�b�N����𒴂������Ȃ�A�擾�͂ł��Ȃ��B
			if( grabNum + resultNum > m_grabed->GetStackLimit() ){
				return;
			}

			m_grabed->SetNumber( grabNum + resultNum );
			m_inventory.SetItem( resultSlot, nullptr );

			//�����i��������̂ŁA�N���t�g�e�[�u���ɒu���Ă���A�C�e���������B
			for (int i = 0; i <= 3; i++) {
				auto& item = m_inventory.GetItem(i);

				if (item) {
					item->SetNumber(item->GetNumber() - 1);

					if (item->GetNumber() == 0) {
						m_inventory.SetItem(i, nullptr);
					}
				}
			}
		}

		if (slotNo < resultSlot) {
			//�ʏ�̃N���b�N����B
			InventoryController::OnClickSlot(event, slotNo);
		}

		//�N���t�g���ʂ������B���ʕ��X���b�g���X�V����B
		auto& rm = RecipeManager::Instance();

		int itemArray[4];
		for (int i = 0; i < 4; i++) {

			auto& item = m_inventory.GetItem(i);

			if (item) {
				itemArray[i] = item->GetID();
			}
			else {
				//null���A�C�e��������\��enum�ɒu��������B
				itemArray[i] = enCube_None;
			}

		}

		auto result = rm.GetResult(itemArray);

		m_inventory.SetItem(4, std::move(result));
	}
}