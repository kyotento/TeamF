#pragma once

#include "BlockType.h"

enum EnItem{
	enItem_Rod = enCube_Num,
	enItem_Coal,
	enItem_Charcoal,
	enItem_Iron_Ingot,
	enItem_Gold_Ingot,
	enItem_Diamond,

	//木のツール類
	enItem_Wood_Sword,
	enItem_Wood_Pickaxe,
	enItem_Wood_Shovel,
	enItem_Wood_Axe,
	enItem_Wood_Hoe,

	//石のツール類
	enItem_Stone_Sword,
	enItem_Stone_Pickaxe,
	enItem_Stone_Shovel,
	enItem_Stone_Axe,
	enItem_Stone_Hoe,

	//鉄のツール類
	enItem_Iron_Sword,
	enItem_Iron_Pickaxe,
	enItem_Iron_Shovel,
	enItem_Iron_Axe,
	enItem_Iron_Hoe,

	//金のツール類
	enItem_Gold_Sword,
	enItem_Gold_Pickaxe,
	enItem_Gold_Shovel,
	enItem_Gold_Axe,
	enItem_Gold_Hoe,

	//ダイヤのツール類
	enItem_Diamond_Sword,
	enItem_Diamond_Pickaxe,
	enItem_Diamond_Shovel,
	enItem_Diamond_Axe,
	enItem_Diamond_Hoe,

	//ブロックを含むすべてのアイテムの数
	enAllItem_Num,
};

//アイテムの属性を決める、
enum EnTool
{
	enTool_None = -1,		//何もなし。
	enTool_Sword,		//剣。
	enTool_Pickaxe,		//ピッケル。
	enTool_Shovel,		//シャベル。
	enTool_Axe,			//斧。
	enTool_Hoe,			//鍬。
	enTool_Helmet,		//ヘルメット。
	enTool_Plate,		//プレート。
	enTool_Leggings,	//レギンス(ズボン)。
	enTool_Boots,		//ブーツ。

	enTool_Num,			//属性の数。
};