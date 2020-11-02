//! @file
#pragma once
#include "Item.h"

//! @brief アイテムとその数を持つクラス。
//! @author Takayama
class ItemStack{
public:

	ItemStack(const Item& item , int number = 1) : m_item(item), m_number(number){}

	//! @brief 個数を設定。
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief 個数を取得。
	int GetNumber() const{
		return m_number;
	}

	//! @brief アイテム情報を取得。
	const Item& GetItem() const{
		return m_item;
	}

	//! @brief アイテムidを取得。
	unsigned GetID() const{
		return m_item.GetID();
	}

	//! @brief スタック上限を取得。
	int GetStackLimit() const{
		return m_item.GetStackLimit();
	}

private:
	const Item& m_item;
	int m_number = 0;
};

