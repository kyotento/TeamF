//! @file
#pragma once
#include "Item.h"

class BlockInfo;

//! @brief アイテム情報管理のために使用。シングルトン。
//! @author Takayama
class ItemDictionary{
public:

	static ItemDictionary& Instance(){
		static ItemDictionary st_storage;
		return st_storage;
	}

	//! @brief フォルダを指定してjsonからアイテムデータを読み込む。
	void LoadItems(std::filesystem::path folder);

	//! @brief BlockInfoからアイテムデータを作成する。
	//! @detail BlockFactory::GetBlockMap() を使用して引数に使ってください。
	void LoadBlocks( const std::unordered_map<EnCube, BlockInfo>& blockMap );

	//! @brief アイテムをid番号から取得。
	//! @param id EnCube か EnItem
	Item& GetItem( int id ){
		return m_array[id];
	}

	//! @brief アイテムをid文字列から取得。
	//! @param id EnItem、EnCubeをそのまま文字列にしたもの。
	//! @exception std::out_of_range 指定されたid文字列が存在しない場合。
	Item& GetItem( const std::string& id ) noexcept(false);

	void SetToolMap();

	EnTool DetermineToolId(std::string itemid);
private:
	ItemDictionary() = default;
	ItemDictionary( const ItemDictionary& is ) = delete;

	std::array<Item, enAllItem_Num> m_array{};

	std::unordered_map<std::string, Item*> m_nameMap;

    const char* m_toolMap[enTool_Num];

	bool m_itemLoaded = false;
	bool m_blockLoaded = false;
};

