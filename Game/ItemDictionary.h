//! @file
#pragma once
#include "Item.h"

//! @brief �A�C�e�����Ǘ��̂��߂Ɏg�p�B�V���O���g���B
//! @author Takayama
class ItemDictionary{
public:

	static ItemDictionary& Instance(){
		static ItemDictionary st_storage;
		return st_storage;
	}

	void LoadItems(std::filesystem::path folder);

	//! @brief �A�C�e����id�ԍ�����擾�B
	//! @param id EnCube �� EnItem
	Item& GetItem( unsigned int id ){
		return m_array[id];
	}

	//! @brief �A�C�e����id�����񂩂�擾�B
	//! @param id EnItem�AEnCube�����̂܂ܕ�����ɂ������́B
	//! @exception std::out_of_range �w�肳�ꂽid�����񂪑��݂��Ȃ��ꍇ�B
	Item& GetItem( const std::string& id ) noexcept(false);

private:
	ItemDictionary();
	ItemDictionary( const ItemDictionary& is ) = delete;

	std::array<Item, enAllItem_Num> m_array{};

	std::unordered_map<std::string, Item*> m_nameMap;

	void AddBlockWithStrID( EnCube id, const char* stringId, const wchar_t * name );
};

