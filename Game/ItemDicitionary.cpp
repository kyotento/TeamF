#include "stdafx.h"
#include "ItemDictionary.h"
#include "BlockFactory.h"

Item& ItemDictionary::GetItem( const std::string & id ){
	return *( m_nameMap.at( id ) );
}

void ItemDictionary::AddBlockWithStrID( EnCube id, const char* stringId, const wchar_t * name ){
	m_array[id] = Item( id, name, 64, BlockFactory::GetModelPath( id ) );
	m_nameMap.emplace( stringId, &m_array[id] );
}

ItemDictionary::ItemDictionary(){

	//enum名文字列と合わせて登録するためのマクロ。
#define AddBlock(id, name) AddBlockWithStrID(id, #id, name);

	//ブロックからアイテム化したアイテムを登録。
	AddBlock( enCube_Grass, L"草" );
	AddBlock( enCube_Soil, L"土" );
	AddBlock( enCube_Stone, L"石" );
	AddBlock( enCube_CobbleStone, L"丸石" );
	AddBlock( enCube_OakLeaf, L"葉" );
	AddBlock( enCube_OakLog, L"オークの原木" );
	AddBlock( enCube_OakWood, L"オークの木材" );
	AddBlock( enCube_CoalOre, L"石炭鉱石" );
	AddBlock( enCube_IronOre, L"鉄鉱石" );
	AddBlock( enCube_GoldOre, L"金鉱石" );
	AddBlock( enCube_CraftingTable, L"作業台" );
	AddBlock( enCube_TorchBlock, L"松明ブロック" );
	AddBlock( enCube_Torch, L"松明");
	AddBlock( enCube_GrassHalf, L"草ハーフ" );

	//アセットが入ったフォルダ。
	std::filesystem::path texDir;
	std::filesystem::path modelDir;

	//スタック上限。
	int stackLimit;

	//アイテムを登録する関数オブジェクト。
	auto AddItemWithStrID =
		[&]( EnItem id, const char* stringId, const wchar_t * name, std::string resource, bool haveModel = false){

		std::filesystem::path modelPath = "Resource/modelData/tools/Wood_Sword.tkm";
		if( haveModel ){
			modelPath.assign(modelDir / ( resource + ".tkm" ));
		}
		m_array[id] = Item( id, name, stackLimit, texDir / (resource + ".dds"), modelPath);
		m_nameMap.emplace( stringId, &( m_array[id] ) );
	};

	//enum名文字列と合わせて登録するためのマクロ。
#define AddItem(id, name, ...) AddItemWithStrID(id, #id, name, __VA_ARGS__);

	//ブロックではないアイテムを登録。

	//ツールではないアイテム類。
	stackLimit = 64;
	texDir.assign( "Resource/spriteData/items" );

	AddItem( enItem_Rod, L"棒", "Rod" );
	AddItem( enItem_Coal, L"石炭", "Coal" );
	AddItem( enItem_Charcoal, L"木炭", "Charcoal" );
	AddItem( enItem_Iron_Ingot, L"鉄インゴッド", "Iron_Ingot" );
	AddItem( enItem_Gold_Ingot, L"金インゴッド", "Gold_Ingot" );
	AddItem( enItem_Diamond, L"ダイアモンド", "Diamond" );

	//ツール類
	stackLimit = 1;
	texDir.assign( "Resource/spriteData/tools" );
	modelDir.assign( "Resource/modelData/tools" );

	//木のツール類
	AddItem( enItem_Wood_Sword, L"木の剣", "Wood_Sword", true);
	AddItem( enItem_Wood_Pickaxe, L"木のピッケル", "Wood_Pickaxe" );
	AddItem( enItem_Wood_Shovel, L"木のシャベル", "Wood_Shovel" );
	AddItem( enItem_Wood_Axe, L"木のオノ", "Wood_Axe");
	AddItem( enItem_Wood_Hoe, L"木のクワ", "Wood_Hoe");

	//石のツール類
	AddItem( enItem_Stone_Sword, L"石の剣", "Stone_Sword", true );
	AddItem( enItem_Stone_Pickaxe, L"石のピッケル", "Stone_Pickaxe");
	AddItem( enItem_Stone_Shovel, L"石のシャベル", "Stone_Shovel");
	AddItem( enItem_Stone_Axe, L"石のオノ", "Stone_Axe");
	AddItem( enItem_Stone_Hoe, L"石のクワ", "Stone_Hoe");

	//鉄のツール類
	AddItem( enItem_Iron_Sword, L"鉄の剣", "Iron_Sword", true );
	AddItem( enItem_Iron_Pickaxe, L"鉄のピッケル", "Iron_Pickaxe" );
	AddItem( enItem_Iron_Shovel, L"鉄のシャベル", "Iron_Shovel");
	AddItem( enItem_Iron_Axe, L"鉄のオノ", "Iron_Axe");
	AddItem( enItem_Iron_Hoe, L"鉄のクワ", "Iron_Hoe");

	//金のツール類
	AddItem( enItem_Gold_Sword, L"金の剣", "Gold_Sword", true );
	AddItem( enItem_Gold_Pickaxe, L"金のピッケル", "Gold_Pickaxe");
	AddItem( enItem_Gold_Shovel, L"金のシャベル", "Gold_Shovel");
	AddItem( enItem_Gold_Axe, L"金のオノ", "Gold_Axe");
	AddItem( enItem_Gold_Hoe, L"金のクワ", "Gold_Hoe");

	//ダイヤのツール類
	AddItem( enItem_Diamond_Sword, L"ダイヤの剣", "Diamond_Sword", true );
	AddItem( enItem_Diamond_Pickaxe, L"ダイヤのピッケル", "Diamond_Pickaxe");
	AddItem( enItem_Diamond_Shovel, L"ダイヤのシャベル", "Diamond_Shovel");
	AddItem( enItem_Diamond_Axe, L"ダイヤのオノ", "Diamond_Axe" );
	AddItem( enItem_Diamond_Hoe, L"ダイヤのクワ", "Diamond_Hoe" );
}
