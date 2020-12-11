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

bool Recipe::ArrayEqualsMirror( const std::vector<int>& array ){
	if( array.size() != m_itemArray.size() ){
		return false;
	}

	const int arraySize = m_itemArray.size();
	const int width = GetWH().first;

	for( int i = 0; i < arraySize; i++ ){
		
		//s‚Ì”½‘Î‘¤‚ðŒvŽZ‚·‚éB
		const int mirror = i - i % width + (width - 1) - i % width;

		if( array[i] != m_itemArray[mirror] ){
			return false;
		}
	}

	return true;
}
