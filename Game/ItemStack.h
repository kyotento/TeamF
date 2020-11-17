//! @file
#pragma once
#include "Item.h"

//! @brief アイテムとその数を持つクラス。コピー可能。
//! @author Takayama
class ItemStack{
public:

	ItemStack(Item& item , int number = 1) : m_item(item), m_number(number){}

	bool operator==( const ItemStack& rhs ){
		return m_number == rhs.m_number && &m_item == &( rhs.m_item );
	}

	bool operator!=( const ItemStack& rhs ){
		return !( operator==( rhs ) );
	}

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

	//! @brief アイテム情報を取得。
	Item& GetItem(){
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

	//! @brief 描画関数
	void Draw( const CVector2& pos, const CVector2& parentScale );

private:
	CFont m_font;
	Item& m_item;
	int m_number = 0;
};

