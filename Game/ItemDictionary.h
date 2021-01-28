//! @file
#pragma once
#include "Item.h"

//! @brief アイテム情報管理のために使用。シングルトン。
//! @author Takayama
class ItemDictionary{
public:

	static ItemDictionary& Instance(){
		static ItemDictionary st_storage;
		return st_storage;
	}

	void LoadItems(std::filesystem::path folder);

	//! @brief アイテムをid番号から取得。
	//! @param id EnCube か EnItem
	Item& GetItem( unsigned int id ){
		return m_array[id];
	}

	//! @brief アイテムをid文字列から取得。
	//! @param id EnItem、EnCubeをそのまま文字列にしたもの。
	//! @exception std::out_of_range 指定されたid文字列が存在しない場合。
	Item& GetItem( const std::string& id ) noexcept(false);

private:
	ItemDictionary();
	ItemDictionary( const ItemDictionary& is ) = delete;

	std::array<Item, enAllItem_Num> m_array{};

	std::unordered_map<std::string, Item*> m_nameMap;

	void AddBlockWithStrID( EnCube id, const char* stringId, const wchar_t * name );
};

