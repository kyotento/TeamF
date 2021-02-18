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

	//スタック上限
	const int stackLimit = item->GetStackLimit();

	int number = 0;
	for( auto& slot : m_slotArray ){
		if (number > 35)
		{
			continue;
		}
		//同じ種類のアイテムが入ってたら
		if(slot && slot->GetID() == item->GetID() ){
			int tempNum = slot->GetNumber() + item->GetNumber();

			//アイテム溢れたら
			if( tempNum > stackLimit ){
				slot->SetNumber( stackLimit );
				item->SetNumber( tempNum - stackLimit );
			} else{
				//溢れなかったら
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
		//空いているスロットがあればそこに入れる。
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

	//どっちにも何もなければ何もしない。
	if( !cursor && !slot ){
		return;
	}

	//スロットが空か、カーソルが空か、アイテムIDが異なる場合は、両者を入れ替える。
	if( !slot || !cursor || slot->GetID() != cursor->GetID() ){
		slot.swap( cursor );
		return;
	}

	//同じ種類のアイテムが入ってたら
	int tempNum = slot->GetNumber() + cursor->GetNumber();
	const int stackLimit = slot->GetStackLimit();

	//アイテム溢れたら
	if( tempNum > stackLimit ){
		slot->SetNumber( stackLimit );
		cursor->SetNumber( tempNum - stackLimit );
	} else{
		//溢れなかったら
		slot->SetNumber( tempNum );
		cursor.reset();
	}
}

void Inventory::RClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor ){
	auto& slot = m_slotArray[slotNo];

	//どっちにも何もなければ何もしない。
	if( !cursor && !slot ){
		return;
	}

	//カーソルが何も掴んでなくて、スロットにアイテムがある場合、半分だけ取る。
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

	//カーソルにアイテムがあって、スロットにアイテムがない場合、1個だけ置く。
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

	//カーソルとスロットが同じアイテムなら1個だけ置く。
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

	//カーソルとスロットが違うアイテムなら両者を交換する。
	cursor.swap( slot );
}

//! @brief ファイル読みこみで、古いフォーマットを読まないために。
const int16_t Inventory::VERSION = 0;

void Inventory::ReadData( std::ifstream & ifs ){
	int16_t fileVersion;
	ifs.read( reinterpret_cast<char*>( &fileVersion ), sizeof( VERSION ) );

	//バージョン違いはエラー
	if( fileVersion != VERSION ){
		MessageBoxA( NULL, "インベントリのバージョンが違います。\n"
					 "チェストなどが読み込みに失敗したため、\nワールドデータを削除してください。",
					 "インベントリデータ読み込み失敗", MB_OK );
		abort();
	}

	size_t size;
	ifs.read( reinterpret_cast<char*>( &size ), sizeof( size ) );

	for( size_t i = 0; i < size; i++ ){
		int16_t id;
		uint8_t num;

		//idと数を取得
		ifs.read( reinterpret_cast<char*>( &id ), sizeof( id ) );
		ifs.read( reinterpret_cast<char*>( &num ), sizeof( num ) );

		if( id == enCube_None ){
			continue;
		}

		//アイテムを作成して入れる。
		m_slotArray[i] = std::make_unique<ItemStack>( Item::GetItem( id ), num );
	}
}

void Inventory::WriteData( std::ofstream & ofs ) const{
	//インベントリのバージョンを出力。
	ofs.write( reinterpret_cast<const char*>( &VERSION ), sizeof( VERSION ) );

	//インベントリのサイズ。
	size_t size = m_slotArray.size();
	ofs.write( reinterpret_cast<const char*>( &size ), sizeof( size ) );

	for( const std::unique_ptr<ItemStack>& item : m_slotArray ){

		int16_t id = enCube_None;
		uint8_t num = 0;
		
		if( item != nullptr ){
			id = item->GetID();
			num = item->GetNumber();
		}

		//idと数を出力
		ofs.write( reinterpret_cast<const char*>( &id ), sizeof( id ) );
		ofs.write( reinterpret_cast<const char*>( &num ), sizeof( num ) );
	}
}
