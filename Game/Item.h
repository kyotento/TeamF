//! @file
#pragma once

#include "ItemType.h"

//! @brief アイテムの種類を表すクラス。 #GetItem(unsigned) で取得する。
//! @details あくまで種類を表すもので、実際のアイテムは ItemStack で扱う。
//! @author Takayama
class Item{
public:

	//! @brief アイテムidからアイテムを取得。
	//! @details ブロックidもアイテムidとして扱うことができる。
	static Item& GetItem(unsigned id);

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

private:
	Item(){}
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber )
		:m_id( enCube ), m_itemName( itemName ), m_limitNumber( limitNumber ){}
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber )
		:m_id( enItem ), m_itemName( itemName ), m_limitNumber( limitNumber ){}

	//! アイテムID
	unsigned m_id = enCube_None;

	//! スタック上限
	int m_limitNumber = 64;

	//! アイテム名
	const wchar_t* m_itemName = nullptr;
};