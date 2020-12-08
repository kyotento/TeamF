//! @file
#pragma once

#include "ItemType.h"

class ItemImage;

//! @brief アイテムの種類を表すクラス。 #GetItem(unsigned) で取得する。
//! @details あくまで種類を表すもので、実際のアイテムは ItemStack で扱う。
//! @author Takayama
class Item{
	friend class ItemDictionary;
public:
	~Item();

	const Item& operator=( Item&& item );

	//! @brief アイテムidからアイテムを取得。
	//! @details ブロックidもアイテムidとして扱うことができる。
	static Item& GetItem( unsigned id );

	//! @brief このアイテムのidを取得。
	unsigned GetID() const{
		return m_id;
	}

	//! @brief スタック上限を取得。
	int GetStackLimit() const{
		return m_limitNumber;
	}

	//! @brief アイテム名を取得。
	const wchar_t* GetItemName() const{
		return m_itemName;
	}

	//! @brief GUI用のアイテムの描画。
	void Draw( const CVector2& pos, const CVector2& scale );

private:
	Item();
	Item( Item&& item );
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber, const std::filesystem::path& modelPath );
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber, const std::filesystem::path& spritePath );

	//! アイテムID
	unsigned m_id = enCube_None;

	//! スタック上限
	int m_limitNumber = 64;

	//! アイテム名
	const wchar_t* m_itemName = nullptr;

	//アイテム画像、またはモデル。
	std::unique_ptr<ItemImage> m_image;
};