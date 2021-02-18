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

std::unique_ptr<ItemStack> Inventory::TakeAllItem(unsigned slotNo) {
	if (m_slotArray[slotNo]) {
		return ItemStack::TakeItem(m_slotArray[slotNo], m_slotArray[slotNo].get()->GetNumber());
	}
	return std::unique_ptr<ItemStack>();
}


std::unique_ptr<ItemStack> Inventory::TakeItem( unsigned slotNo, int num ){
	if( m_slotArray[slotNo] ){
		return ItemStack::TakeItem( m_slotArray[slotNo], num );
	}
	return std::unique_ptr<ItemStack>();
}

void Inventory::DeleteItem( unsigned slotNo, int num ){
	if( m_slotArray[slotNo] ){
		auto& item = m_slotArray[slotNo];
		item->SetNumber( item->GetNumber() - num );

		if( item->GetNumber() <= 0 ){
			item.reset();
		}
	}
}

bool Inventory::AddItem( std::unique_ptr<ItemStack>& item ){
	if( item == nullptr ){
		return false;
	}

	//�X�^�b�N���
	const int stackLimit = item->GetStackLimit();

	int number = 0;
	for( auto& slot : m_slotArray ){
		if (number > 35)
		{
			continue;
		}
		//������ނ̃A�C�e���������Ă���
		if(slot && slot->GetID() == item->GetID() ){
			int tempNum = slot->GetNumber() + item->GetNumber();

			//�A�C�e����ꂽ��
			if( tempNum > stackLimit ){
				slot->SetNumber( stackLimit );
				item->SetNumber( tempNum - stackLimit );
			} else{
				//���Ȃ�������
				slot->SetNumber( tempNum );
				item.reset();
				return true;
			}
		}
		number++;
	}
	number = 0;
	for (auto& slot : m_slotArray) {
		if (number > 35)
		{
			return false;
		}
		//�󂢂Ă���X���b�g������΂����ɓ����B
		if (!slot) {
			slot.swap(item);
			return true;
		}
		number++;
	}
	
	return false;
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

//! @brief �t�@�C���ǂ݂��݂ŁA�Â��t�H�[�}�b�g��ǂ܂Ȃ����߂ɁB
const int16_t Inventory::VERSION = 0;

void Inventory::ReadData( std::ifstream & ifs ){
	int16_t fileVersion;
	ifs.read( reinterpret_cast<char*>( &fileVersion ), sizeof( VERSION ) );

	//�o�[�W�����Ⴂ�̓G���[
	if( fileVersion != VERSION ){
		MessageBoxA( NULL, "�C���x���g���̃o�[�W�������Ⴂ�܂��B\n"
					 "�`�F�X�g�Ȃǂ��ǂݍ��݂Ɏ��s�������߁A\n���[���h�f�[�^���폜���Ă��������B",
					 "�C���x���g���f�[�^�ǂݍ��ݎ��s", MB_OK );
		abort();
	}

	size_t size;
	ifs.read( reinterpret_cast<char*>( &size ), sizeof( size ) );

	for( size_t i = 0; i < size; i++ ){
		int16_t id;
		uint8_t num;

		//id�Ɛ����擾
		ifs.read( reinterpret_cast<char*>( &id ), sizeof( id ) );
		ifs.read( reinterpret_cast<char*>( &num ), sizeof( num ) );

		if( id == enCube_None ){
			continue;
		}

		//�A�C�e�����쐬���ē����B
		m_slotArray[i] = std::make_unique<ItemStack>( Item::GetItem( id ), num );
	}
}

void Inventory::WriteData( std::ofstream & ofs ) const{
	//�C���x���g���̃o�[�W�������o�́B
	ofs.write( reinterpret_cast<const char*>( &VERSION ), sizeof( VERSION ) );

	//�C���x���g���̃T�C�Y�B
	size_t size = m_slotArray.size();
	ofs.write( reinterpret_cast<const char*>( &size ), sizeof( size ) );

	for( const std::unique_ptr<ItemStack>& item : m_slotArray ){

		int16_t id = enCube_None;
		uint8_t num = 0;
		
		if( item != nullptr ){
			id = item->GetID();
			num = item->GetNumber();
		}

		//id�Ɛ����o��
		ofs.write( reinterpret_cast<const char*>( &id ), sizeof( id ) );
		ofs.write( reinterpret_cast<const char*>( &num ), sizeof( num ) );
	}
}
