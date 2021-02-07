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

	//enCube_None用のInfo
	static const BlockInfo NONE_BLOCK;
};