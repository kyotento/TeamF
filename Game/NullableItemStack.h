//! @file
#pragma once

#include "BlockType.h"

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

	//! @brief アイテムidを取得。
	virtual unsigned GetID() const{
		return enCube_None;
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