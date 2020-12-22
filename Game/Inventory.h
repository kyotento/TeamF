//! @file
#pragma once

class NullableItemStack;
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

	//! @brief アイテムを取得。
	std::unique_ptr<ItemStack>& GetItem( unsigned slotNo ){
		return m_slotArray[slotNo];
	}

	//! @brief アイテムを取得。指定したスロットがnullなら、nullオブジェクトを返す。
	NullableItemStack& GetNullableItem( unsigned slotNo );

	//! @brief 指定した数だけ、スロットからアイテムを取る。
	//! @details 普通のGetは参照の取得で、こっちはインベントリから戻り値へアイテムを移動させる。
	std::unique_ptr<ItemStack> TakeItem( unsigned slotNo, int num );

	//! @brief 指定した数だけスロットからアイテムを消す。
	void DeleteItem( unsigned slotNo, int num );

	auto begin(){
		return m_slotArray.begin();
	}

	auto end(){
		return m_slotArray.end();
	}

	//! @brief アイテムを追加する。アイテムを拾う操作に使用する。
	//! @param cursor[in,out] 追加するアイテム。処理後はemptyになりますが、アイテムが溢れたら溢れた分が残ります。
	void AddItem( std::unique_ptr<ItemStack>& item );

	//! @brief スロットに対して左クリックしたときの操作。
	//! @param slotNo スロット番号
	//! @param cursor[in, out] カーソルが持っているアイテム。
	void LClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

	//! @brief スロットに対して右クリックしたときの操作。
	//! @param slotNo スロット番号
	//! @param cursor[in, out] カーソルが持っているアイテム。
	void RClickSlot( unsigned slotNo, std::unique_ptr<ItemStack>& cursor );

private:
	//! インベントリの長さ。
	static const unsigned m_inventryWidth = 9;
	std::vector<std::unique_ptr<ItemStack>>  m_slotArray;

	CFont m_font;
};

