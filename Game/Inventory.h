//! @file
#pragma once

class ItemStack;

//! @brief インベントり。アイテムを入れる倉庫。
//! @author Takayama
class Inventory{
public:
	//! @param size スロットの数
	Inventory( unsigned size );
	~Inventory();

	//! @brief アイテムをスロットにセット。スロットを上書きします。
	void SetItem( unsigned slotNo, std::unique_ptr<ItemStack> item );

	//! @brief カーソルの手中にアイテムをセット。元から掴んでいる物を上書きします。
	void SetGrabItem( std::unique_ptr<ItemStack> item );

	//! @brief アイテムを取得。
	ItemStack* GetItem( unsigned slotNo ){
		return m_slotArray[slotNo].get();
	}

	//! @brief カーソルが掴んでいるアイテムを取得。
	ItemStack* GetGrabItem(){
		return m_grabedItem.get();
	}

	//! @brief アイテムを追加する。アイテムを拾う操作に使用する。
	//! @param cursor[in,out] 追加するアイテム。処理後はemptyになりますが、アイテムが溢れたら溢れた分が残ります。
	void AddItem( std::unique_ptr<ItemStack>& item );

	//! @brief スロットに対して左クリックしたときの操作。
	//! @param slotNo スロット番号
	void LClickSlot( unsigned slotNo );

	//! @brief スロットに対して右クリックしたときの操作。
	//! @param slotNo スロット番号
	void RClickSlot( unsigned slotNo );

private:
	//! インベントリの長さ。
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;

	//! カーソルが掴んでいるアイテム。
	std::unique_ptr<ItemStack> m_grabedItem;
	CFont m_font;
};

