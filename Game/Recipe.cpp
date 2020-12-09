#include "stdafx.h"
#include "Recipe.h"

Recipe::Recipe( int width, int height, const std::vector<int>& itemArray, const ItemStack& result ):
	m_widthHeight(width, height), m_itemArray(itemArray), m_result(result){}

Recipe::~Recipe(){}

bool Recipe::operator==( const Recipe & rhs ){
	return m_widthHeight == rhs.m_widthHeight && m_result == rhs.m_result && ArrayEquals(rhs.m_itemArray);
}

bool Recipe::ArrayEquals( const std::vector<int>& array ){
	const auto& l = m_itemArray;
	const auto& r = array;
	return std::equal( l.begin(), l.end(), r.begin(), r.end() );
}
