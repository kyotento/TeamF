//! @file
#pragma once

//! @brief Block の種類。
enum EnCube
{
	enCube_None = -1,

	enCube_Grass,
	enCube_Soil,
	enCube_Stone,			//石。
	enCube_CobbleStone,		//丸石。
	enCube_OakLog,			//オークの原木。
	enCube_OakWood,			//オークの木材。
	enCube_OakLeaf,			//オークの葉っぱ。
	enCube_CoalOre,			//石炭鉱石。
	enCube_IronOre,			//鉄鉱石。
	enCube_GoldOre,			//金鉱石。
	enCube_Bedrock,			//岩盤。
	enCube_CraftingTable,	//クラフティングテーブル。
	enCube_TorchBlock,		//松明ブロック
	enCube_Torch,			//松明
	enCube_GrassHalf,		//草ハーフ
	enCube_GrassStairs,		//草階段
	enCube_WoGBlock,		//ラスゴブロック
	enCube_Glass,			//ガラス。
	enCube_DoorUp,			//ドア(上部分)	
	enCube_DoorDown,		//ドア(下部分)
	enCube_BedHead,			//ベッド(頭側)
	enCube_BedLeg,			//ベッド(足側)
	enCube_Chest,			//チェスト。
	enCube_Sand,			//砂。
	enCube_Furnace,			//かまど。
	enCube_TNT,				//TNT。
	enCube_IronBlock,		//鉄ブロック。
	enCube_GoldBlock,		//石炭ブロック。
	enCube_DiamondOre,		//ダイアモンド鉱石。
	enCube_DiamondBlock,	//ダイアモンドブロック
	enCube_CoalBlock,		//石炭ブロック。
	enCube_OakWoodHalf,		//木のハーフブロック。
	enCube_OakWoodStairs,	//木の階段。
	enCube_StoneHalf,		//石のハーフブロック。
	enCube_StoneStairs,		//石の階段。
	enCube_CobbleStoneHalf,	//丸石のハーフブロック。
	enCube_CobbleStoneStairs,//丸石の階段。
	enCube_StoneBrick,		//石レンガ。
	enCube_StoneBrickStairs,//石レンガの階段。
	enCube_StoneBrickHalf,	//石レンガのハーフブロック。
	enCube_PatternedStoneBrick,//模様付き石レンガ。
	enCube_SandStone,			//砂岩。
	enCube_SmoothSandStone,		//なめらかな砂岩。
	enCube_PatternedSandBlock,	//模様付き砂岩。
	enCube_SandStoneStairs,		//砂岩の階段。
	enCube_SandStoneHalf,		//砂岩のハーフブロック。
	enCube_LearnTorch,			//斜め松明

	enCube_Num
};