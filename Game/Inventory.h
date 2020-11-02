//! @file
#pragma once
	
class ItemStack;

//! @brief インベントり。アイテムを入れる倉庫。
//! @author Takayama
class Inventory
{
public:
	//! @param size スロットの数
	Inventory(unsigned size);
	~Inventory();

	//! @brief アイテムを追加する。アイテムを拾う操作に使用する。
	//! @param cursor[in,out] 追加するアイテム。処理後はemptyになりますが、アイテムが溢れたら溢れた分が残ります。
	void AddItem( std::unique_ptr<ItemStack>& item);

	//! @brief スロットに対して左クリックしたときの操作。
	//! @param slotNo スロット番号
	//! @param cursor[in,out] カーソルが保持しているアイテム。この関数はこの参照を編集します。
	void LClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

	//! @brief スロットに対して右クリックしたときの操作。
	//! @param slotNo スロット番号
	//! @param cursor[in,out] カーソルが保持しているアイテム。この関数はこの参照を編集します。
	void RClickItem( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

private:
	//インベントリの長さ
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;
	CFont m_font;
};

