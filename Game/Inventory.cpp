#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemStack.h"

Inventory::Inventory(unsigned size){
	m_slotArray.resize( size );
}

Inventory::~Inventory(){}

void Inventory::AddItem( std::unique_ptr<ItemStack>& item ){
	//�X�^�b�N���
	const int stackLimit = item->GetStackLimit();

	for( auto& slot : m_slotArray ){
		//�󂢂Ă���X���b�g������΂����ɓ���ďI���B
		if( !slot ){
			slot.swap( item );
			return;
		}

		//������ނ̃A�C�e���������Ă���
		if( slot->GetID() == item->GetID() ){
			int tempNum = slot->GetNumber() + item->GetNumber();

			//�A�C�e����ꂽ��
			if( tempNum > stackLimit ){
				slot->SetNumber( stackLimit );
				item->SetNumber( tempNum - stackLimit );
			} else{
				//���Ȃ�������
				slot->SetNumber( tempNum );
				item.reset();
				return;
			}
		}
	}
}

void Inventory::LClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& item ){
	auto& slot = m_slotArray[slotNo];

	//�X���b�g���󂩁A�A�C�e��ID���قȂ�ꍇ�́A���҂����ւ���B
	if( !slot || slot->GetID() != item->GetID() ){
		slot.swap( item );
		return;
	}

	//������ނ̃A�C�e���������Ă���
	int tempNum = slot->GetNumber() + item->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//�A�C�e����ꂽ��
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		item->SetNumber( tempNum - stackLimit );
	} else{
		//���Ȃ�������
		slot->SetNumber( tempNum );
		item.reset();
	}
}

void Inventory::RClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& item ){
	auto& slot = m_slotArray[slotNo];

	//�ǂ����ɂ������Ȃ���Ή������Ȃ��B
	if( !item && !slot ){
		return;
	}

	//�J�[�\���������͂�łȂ��āA�X���b�g�ɃA�C�e��������ꍇ�A�����������B
	if( !item && slot){
		int num = slot->GetNumber();
		int half = num / 2;

		item = std::make_unique<ItemStack>( slot->GetItem(), num - half );
		slot->SetNumber( half );
		return;
	}

	//�J�[�\���ɃA�C�e���������āA�X���b�g�ɃA�C�e�����Ȃ��ꍇ�A1�����u���B
	if( item && !slot ){
		int num = item->GetNumber() - 1;
		if( num > 0 ){
			item->SetNumber( num );
		} else{
			item.reset();
		}
		slot = std::make_unique<ItemStack>( item->GetItem(), 1 );
		return;
	}

	//�J�[�\���ƃX���b�g�������A�C�e���Ȃ�1�����u���B
	if( item->GetID() == slot->GetID() ){
		if( slot->GetNumber() == slot->GetStackLimit() ){
			return;
		}

		int num = item->GetNumber() - 1;
		if( num > 0 ){
			item->SetNumber( num );
		} else{
			item.reset();
		}
		slot->SetNumber( slot->GetNumber() + 1 );
		return;
	}

	//�J�[�\���ƃX���b�g���Ⴄ�A�C�e���Ȃ痼�҂���������B
	item.swap( slot );
}

//void Inventory::PostRender()
//{
//	wchar_t output[256];
//	swprintf_s(output, L"  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n  %ls %d\n",m_inventoryList[0]->s_item->GetItemName(),m_inventoryList[0]->s_number, m_inventoryList[1]->s_item->GetItemName(), m_inventoryList[1]->s_number, m_inventoryList[2]->s_item->GetItemName(), m_inventoryList[2]->s_number, m_inventoryList[3]->s_item->GetItemName(), m_inventoryList[3]->s_number,
//		m_inventoryList[4]->s_item->GetItemName(), m_inventoryList[4]->s_number, m_inventoryList[5]->s_item->GetItemName(), m_inventoryList[5]->s_number, m_inventoryList[6]->s_item->GetItemName(), m_inventoryList[6]->s_number, m_inventoryList[7]->s_item->GetItemName(), m_inventoryList[7]->s_number, m_inventoryList[8]->s_item->GetItemName(), m_inventoryList[8]->s_number);
//	m_font.DrawScreenPos(output, { 500.0f,300.0f }, CVector4::White(), { 0.6f,0.6f },
//		CVector2::Zero(),
//		0.0f,
//		DirectX::SpriteEffects_None,
//		0.7f
//	);
//}