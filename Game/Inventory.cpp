#include "stdafx.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemStack.h"
#include "ItemType.h"

Inventory::Inventory( unsigned size ){
	m_slotArray.resize( size );
}

Inventory::~Inventory(){}

void Inventory::SetItem( unsigned slotNo, std::unique_ptr<ItemStack> item ){
	m_slotArray[slotNo] = std::move( item );
}

NullableItemStack & Inventory::GetNullableItem( unsigned slotNo ){
	if( m_slotArray[slotNo] ){
		return *m_slotArray[slotNo];
	} else{
		return NullableItemStack::Instance();
	}
}

void Inventory::AddItem( std::unique_ptr<ItemStack>& item ){
	if( item == nullptr ){
		return;
	}

	//�X�^�b�N���
	const int stackLimit = item->GetStackLimit();

	for( auto& slot : m_slotArray ){
		//�󂢂Ă���X���b�g������΂����ɓ����B
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

void Inventory::LClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor ){
	auto& slot = m_slotArray[slotNo];

	//�ǂ����ɂ������Ȃ���Ή������Ȃ��B
	if( !cursor && !slot ){
		return;
	}

	//�X���b�g���󂩁A�J�[�\�����󂩁A�A�C�e��ID���قȂ�ꍇ�́A���҂����ւ���B
	if( !slot || !cursor || slot->GetID() != cursor->GetID() ){
		slot.swap( cursor );
		return;
	}

	//������ނ̃A�C�e���������Ă���
	int tempNum = slot->GetNumber() + cursor->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//�A�C�e����ꂽ��
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		cursor->SetNumber( tempNum - stackLimit );
	} else{
		//���Ȃ�������
		slot->SetNumber( tempNum );
		cursor.reset();
	}
}

void Inventory::RClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor ){
	auto& slot = m_slotArray[slotNo];

	//�ǂ����ɂ������Ȃ���Ή������Ȃ��B
	if( !cursor && !slot ){
		return;
	}

	//�J�[�\���������͂�łȂ��āA�X���b�g�ɃA�C�e��������ꍇ�A�����������B
	if( !cursor && slot ){
		int num = slot->GetNumber();
		int half = num / 2;

		cursor = std::make_unique<ItemStack>( slot->GetItem(), num - half );

		if( half == 0 ){
			slot.reset();
		} else{
			slot->SetNumber( half );
		}
		return;
	}

	//�J�[�\���ɃA�C�e���������āA�X���b�g�ɃA�C�e�����Ȃ��ꍇ�A1�����u���B
	if( cursor && !slot ){
		slot = std::make_unique<ItemStack>( cursor->GetItem(), 1 );
		int num = cursor->GetNumber() - 1;
		if( num > 0 ){
			cursor->SetNumber( num );
		} else{
			cursor.reset();
		}
		return;
	}

	//�J�[�\���ƃX���b�g�������A�C�e���Ȃ�1�����u���B
	if( cursor->GetID() == slot->GetID() ){
		if( slot->GetNumber() == slot->GetStackLimit() ){
			return;
		}

		int num = cursor->GetNumber() - 1;
		if( num > 0 ){
			cursor->SetNumber( num );
		} else{
			cursor.reset();
		}
		slot->SetNumber( slot->GetNumber() + 1 );
		return;
	}

	//�J�[�\���ƃX���b�g���Ⴄ�A�C�e���Ȃ痼�҂���������B
	cursor.swap( slot );
}