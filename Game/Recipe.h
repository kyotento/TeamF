//! @file
#pragma once
#include "ItemStack.h"

//! @brief クラフトレシピを表現するクラス。
//! @details レシピの、幅、高さ、アイテム配列、成果物を持つ。
//! @details 例えば松明なら、幅1、高さ2、アイテム配列[石炭,棒]、である。
//! @author Takayama
class Recipe{
public:

	Recipe(int width, int height, const std::vector<int>& itemArray, const ItemStack& result);
	~Recipe();

	bool operator==( const Recipe& rhs );

	bool operator!=( const Recipe& rhs ){
		return !(operator==( rhs ));
	}

	//! @brief 幅、高さ、成果物は無視して、アイテム配列だけを比較して同じかどうかを返す。
	bool LightEquals( const Recipe& rhs );

	//! @brief レシピの幅と高さを取得。
	//! @details std::pair の第1要素が幅、第2要素が高さ。
	std::pair<int, int> GetWH(){
		return m_widthHeight;
	}

	//! @brief レシピの成果物を返す。
	std::unique_ptr<ItemStack> GetResult(){
		return std::make_unique<ItemStack>( m_result );
	}

private:
	std::pair<int, int> m_widthHeight;
	std::vector<int> m_itemArray;
	ItemStack m_result;
};

