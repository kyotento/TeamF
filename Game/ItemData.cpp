#include "stdafx.h"
#include "ItemData.h"

ItemData::ItemData()
{
	/*enCube_None = -1,
		enCube_Grass = 0,
		enCube_Soil,
		enCube_Stone,
		enCube_Ore,*/
	m_itemList[enCube_None] = new Item(enCube_None, L"–³‚µ", 0);
	m_itemList[enCube_Grass] = new Item(enCube_Grass, L"‘", 64);
	m_itemList[enCube_Soil] = new Item(enCube_Soil, L"“y", 64);
	m_itemList[enCube_Stone] = new Item(enCube_Stone, L"Î", 64);
	m_itemList[enCube_Ore] = new Item(enCube_Ore, L"zÎ", 64);
}