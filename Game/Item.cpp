#include "stdafx.h"
#include "Item.h"

ItemData::ItemData()
{
	/*enCube_None = -1,
		enCube_Grass = 0,
		enCube_Soil,
		enCube_Stone,
		enCube_Ore,*/
	m_itemList[enCube_None] = new Item(enCube_None, L"ñ≥Çµ", 64);
	m_itemList[enCube_Grass] = new Item(enCube_Grass, L"ëê", 64);
	m_itemList[enCube_Soil] = new Item(enCube_Soil, L"ìy", 64);
	m_itemList[enCube_Stone] = new Item(enCube_Stone, L"êŒ", 64);
	m_itemList[enCube_Leaf] = new Item(enCube_Leaf, L"ót", 64);
	m_itemList[enCube_Ore] = new Item(enCube_Ore, L"çzêŒ", 64);

}