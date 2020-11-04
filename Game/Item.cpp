#include "stdafx.h"
#include "Item.h"

Item& Item::GetItem( unsigned id ){
	struct ItemArray{
		ItemArray(){
			m_array[enCube_None] = Item( enCube_None, L"–³‚µ", 1 );
			m_array[enCube_Grass] = Item( enCube_Grass, L"‘", 64 );
			m_array[enCube_Soil] = Item( enCube_Soil, L"“y", 64 );
			m_array[enCube_Stone] = Item( enCube_Stone, L"Î", 64 );
			m_array[enCube_Leaf] = Item( enCube_Leaf, L"—t", 64 );
			m_array[enCube_IronOre] = Item( enCube_IronOre, L"zÎ", 64 );
		}

		Item& operator[]( int n ){
			return m_array[n];
		}

	private:
		std::array<Item, enAllItem_Num> m_array{};
	};

	static ItemArray st_itemArray;

	return st_itemArray[id];
}
