#pragma once

class ItemStack;

class Inventory
{
public:
	Inventory();
	~Inventory();

	int AddItem( std::unique_ptr<ItemStack> item);

private:
	//�C���x���g���̒���
	static const int m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_inventoryList;
	CFont m_font;
};

