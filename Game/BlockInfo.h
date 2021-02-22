#pragma once
#include "ItemType.h"
#include "BlockType.h"
#include "AABB.h"

//! @brief ブロックの種類ごとに共通の情報。
class BlockInfo{
public:
	//ID
	EnCube id = EnCube::enCube_None;
	//名前
	std::string name;
	//ブロックの耐久値
	int hp = 1;
	//破壊に有効なツール
	EnTool usefulTool = enTool_None;
	//モデルパス
	std::filesystem::path modelPath;
	//AABB
	std::vector<AABB> aabbArray;
	//明るさ
	char light = 0;
	//不透明ならtrue
	bool isOpacity = true;
	//テクスチャに透明要素あるか?
	bool isTransTexture = false;
	//破壊時に落とすアイテム
	EnItem dropItem = EnItem::enAllItem_Num;
	//アイコン用スプライトパス
	std::filesystem::path spritePath;
	//ドロップモデルパス　手に持つときのモデルでもある
	std::filesystem::path dropModelPath;

	//燃料レベル
	float fuel_level = 0;

	//キャラコンと衝突するか
	bool isColision = true;

	//enCube_None用のInfo
	static const BlockInfo NONE_BLOCK;
};