#pragma once
#include "Item.h"

class ItemStack{
public:

	ItemStack(){}

	//! @brief 個数を設定。
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief 個数を取得。
	int GetNumber(){
		return m_number;
	}

	//! @brief アイテム情報を取得。
	Item& GetItem(){
		return m_item;
	}

private:
	Item& m_item;
	int m_number = 0;
};

