#pragma once

#include "BlockType.h"

enum EnItem{
	enItem_Rod = enCube_Num,
	enItem_Coal,
	enItem_Charcoal,
	enItem_Iron_Ingot,
	enItem_Gold_Ingot,
	enItem_Diamond,
	enItem_Leather,//皮

	//食料
	enItem_Raw_Meat,
	enItem_Steak,

	//木のツール類
	enItem_Wood_Sword,
	enItem_Wood_Pickaxe,
	enItem_Wood_Shovel,
	enItem_Wood_Axe,
	enItem_Wood_Hoe,

	//皮のツール類。
	enItem_Leather_Helmet,
	enItem_Leather_ChestPlate,
	enItem_Leather_ChestPlate2,
	enItem_Leather_Leggings,
	enItem_Leather_Boots,


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

	//防具。
	enItem_Iron_Helmet,
	enItem_Iron_ChestPlate,
	enItem_Iron_ChestPlate2,
	enItem_Iron_Leggings,
	enItem_Iron_Boots,


	//金のツール類
	enItem_Gold_Sword,
	enItem_Gold_Pickaxe,
	enItem_Gold_Shovel,
	enItem_Gold_Axe,
	enItem_Gold_Hoe,

	//防具。
	enItem_Gold_Helmet,
	enItem_Gold_ChestPlate,
	enItem_Gold_ChestPlate2,
	enItem_Gold_Leggings,
	enItem_Gold_Boots,

	//ダイヤのツール類
	enItem_Diamond_Sword,
	enItem_Diamond_Pickaxe,
	enItem_Diamond_Shovel,
	enItem_Diamond_Axe,
	enItem_Diamond_Hoe,

	//防具。
	enItem_Diamond_Helmet,
	enItem_Diamond_ChestPlate,
	enItem_Diamond_ChestPlate2,
	enItem_Diamond_Leggings,
	enItem_Diamond_Boots,

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