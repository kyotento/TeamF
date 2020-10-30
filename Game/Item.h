#pragma once
#include "BlockType.h"
#include <unordered_map>

class Item
{
public:
	Item(EnCube enCube, const wchar_t* itemName, int limitNumber) :m_state(enCube), m_itemName(itemName), m_limitNumber(limitNumber)
	{

	}
	~Item() {}
	EnCube GetBlockType()
	{
		return m_state;
	}
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	int GetLimit()
	{
		return m_limitNumber;
	}
	bool GetisLimit(const int number)
	{
		if (number > m_limitNumber) {
			return true;
		}
		else {
			return false;
		}
	}
	void SetItemName(const wchar_t* itemName)
	{
		m_itemName = itemName;
	}
	const wchar_t* GetItemName()
	{
		return m_itemName;
	}
private:
	EnCube m_state = enCube_None;
	int m_limitNumber = 64;
	const wchar_t* m_itemName = nullptr;
};

class ItemData
{
private:
	ItemData();
	~ItemData() {}
public:
	static ItemData& GetInstance()
	{
		static ItemData instance;
		return instance;
	}
	Item* GetItem(EnCube enCube) {
		return m_itemList[enCube];
	}
private:
	std::unordered_map<int, Item*> m_itemList;   //プレイヤーの座標のリスト
};

static inline ItemData& GetItemData()
{
	return ItemData::GetInstance();
}