//! @file
#pragma once

#include "ItemType.h"

class ItemImage;

//! @brief アイテムの種類を表すクラス。 #GetItem(int) で取得する。
//! @details あくまで種類を表すもので、実際のアイテムは ItemStack で扱う。
//! @author Takayama
class Item{
	friend class ItemDictionary;
public:
	~Item();

	const Item& operator=( Item&& item );

	//! @brief アイテムidからアイテムを取得。
	//! @details ブロックidもアイテムidとして扱うことができる。
	static Item& GetItem( int id );

	//! @briefアイテムの文字列idからアイテムを取得。
	//! @param strId EnItem、EnCubeをそのまま文字列にしたもの。
	//! @exception std::out_of_range 指定されたid文字列が存在しない場合。
	static Item& GetItem( const std::string& strId ) noexcept(false);

	//! @brief このアイテムのidを取得。
	int GetID() const{
		return m_id;
	}

	//! @brief このアイテムの属性を取得。
	int GetToolID() const {
		return m_toolId;
	}

	//! @brief このアイテムのツールレベル(木:1～ダイヤ:5)を取得
	int GetToolLevel() const{
		return m_toolLevel;
	}

	//! @brief スタック上限を取得。
	int GetStackLimit() const{
		return m_limitNumber;
	}

	//! @brief アイテム名を取得。
	const wchar_t* GetItemName() const{
		return m_itemName;
	}

	//! @brief GUI用のアイテムの描画。
	void Draw( const CVector2& pos, const CVector2& scale );

	/// <summary>
	/// ブロックかツールかを取得する。
	/// </summary>
	/// <returns>trueならブロック</returns>
	bool GetIsBlock() {
		return m_id < enCube_Num;	
	}

	//! @brief アイテムの3Dモデルのパスを取得。
	std::filesystem::path GetModelPath() const{
		return m_modelPath;
	}

private:
	Item();
	Item( Item&& item );
	Item( EnCube enCube, const wchar_t* itemName, int limitNumber, const std::filesystem::path& modelPath );
	Item( EnItem enItem, const wchar_t* itemName, int limitNumber, const std::filesystem::path& spritePath, const std::filesystem::path& modelPath );

	//! アイテムID
	int m_id = enCube_None;

	//! スタック上限
	int m_limitNumber = 64;

	//! アイテム名
	const wchar_t* m_itemName = nullptr;

	//アイテム画像。
	std::unique_ptr<ItemImage> m_image;

	static constexpr int INSTANCE_MODEL_MAX = 500;

	//3Dモデルのパス。
	std::filesystem::path m_modelPath;

	//アイテムの属性。
	int m_toolId = enTool_None;

	//ツールレベル。木:1からダイヤ:5まで
	int m_toolLevel = 1;
};