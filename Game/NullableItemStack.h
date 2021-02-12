//! @file
#pragma once

#include "ItemType.h"

//! @brief ItemStackの基底クラス。継承されていないこのクラス単体で、nullオブジェクトを表す。
//! @author Takayama
class NullableItemStack{
public:

	static NullableItemStack& Instance(){
		static NullableItemStack nullObj;
		return nullObj;
	}

	//! @brief 個数を取得。
	virtual int GetNumber() const{
		return 0;
	}

	//! @brief スタック上限を取得。
	virtual int GetStackLimit() const{
		return 0;
	}

	//! @brief アイテムidを取得。
	virtual int GetID() const{
		return enCube_None;
	}

	//! @brief アイテムの属性を取得。
	virtual int GetToolID() const {
		return enTool_None;
	}

	//! @brief 食料か
	virtual bool IsFood() const{
		return false;
	}

	//! @brief 燃料か
	virtual bool IsFuel() const{
		return false;
	}

	/// <summary>
	/// ブロックか、ツールかを取得する。
	/// </summary>
	/// <returns>trueならブロック</returns>
	virtual bool GetIsBlock(){
		return false;
	}

	//! @brief 描画関数
	virtual void Draw( const CVector2& pos, const CVector2& parentScale ){}

protected:
	NullableItemStack(){}
};