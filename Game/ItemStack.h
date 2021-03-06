//! @file
#pragma once
#include "NullableItemStack.h"
#include "Item.h"

//! @brief アイテムとその数を持つクラス。コピー可能。
//! @author Takayama
class ItemStack : public NullableItemStack{
public:

	//! @brief アイテムスタックからnum個を別のスタックとして取り出す。
	static std::unique_ptr<ItemStack> TakeItem( std::unique_ptr<ItemStack>& item, int num );

	ItemStack(Item& item , int number = 1) : m_item(item), m_number(number){}

	bool operator==( const ItemStack& rhs ){
		return m_number == rhs.m_number && &m_item == &( rhs.m_item );
	}

	bool operator!=( const ItemStack& rhs ){
		return !( operator==( rhs ) );
	}

	//! @brief 個数を設定。
	void SetNumber( int num ){
		m_number = num;
	}

	//! @brief 個数を取得。
	int GetNumber() const override{
		return m_number;
	}

	//! @brief 個数を増やす。(マイナス値で減らす。)
	void AddNumber( int num){
		m_number += num;
	}

	//! @brief アイテム情報を取得。
	const Item& GetItem() const{
		return m_item;
	}

	//! @brief アイテム情報を取得。
	Item& GetItem(){
		return m_item;
	}

	//! @brief アイテムの3Dモデルのパスを取得。
	std::filesystem::path GetModelPath() const{
		return m_item.GetModelPath();
	}

	//! @brief アイテムidを取得。
	int GetID() const override{
		return m_item.GetID();
	}

	//! @brief アイテムの属性を取得。
	int GetToolID() const override {
		return m_item.GetToolID();
	}

	//! @brief このアイテムのツールレベル(木:1〜ダイヤ:5)を取得
	int GetToolLevel() const {
		return m_item.GetToolLevel();
	}

	//! @brief このアイテムのフードレベル(食料の回復量)を取得
	int GetFoodLevel() const{
		return m_item.GetFoodLevel();
	}

	//! @brief 食料か
	bool IsFood() const override{
		return m_item.IsFood();
	}

	//! @brief このアイテムの燃料レベル(焼ける数)を取得
	float GetFuelLevel() const{
		return m_item.GetFuelLevel();
	}

	//! @brief 燃料か
	bool IsFuel() const override{
		return m_item.IsFuel();
	}

	//! @brief スタック上限を取得。
	int GetStackLimit() const override{
		return m_item.GetStackLimit();
	}

	/// <summary>
	/// ブロックか、ツールかを取得する。
	/// </summary>
	/// <returns>trueならブロック</returns>
	bool GetIsBlock() override{
		return m_item.GetIsBlock();
	}

	//! @brief 描画関数
	void Draw( const CVector2& pos, const CVector2& parentScale ) override;

private:
	CFont m_font;
	Item& m_item;
	int m_number = 0;
};

