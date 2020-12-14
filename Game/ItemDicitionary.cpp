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

	//アイテムを登録する関数オブジェクト。
	std::filesystem::path texDir;

	auto AddItemWithStrID =
		[&]( EnItem id, const char* stringId, const wchar_t * name,
			 const std::filesystem::path & resource, int stackLimit = 64 ){

		m_array[id] = Item( id, name, stackLimit, texDir / resource );
		m_nameMap.emplace( stringId, &( m_array[id] ) );
	};

	//enum名文字列と合わせて登録するためのマクロ。
#define AddItem(id, name, ...) AddItemWithStrID(id, #id, name, __VA_ARGS__);

	//ブロックではないアイテムを登録。
	texDir.assign( "Resource/spriteData/items" );

	AddItem( enItem_Rod, L"棒", "Rod.dds" );
	AddItem( enItem_Coal, L"石炭", "Coal.dds" );
	AddItem( enItem_Charcoal, L"木炭", "Charcoal.dds" );
	AddItem( enItem_Iron_Ingot, L"鉄インゴッド", "Iron_Ingot.dds" );
	AddItem( enItem_Gold_Ingot, L"金インゴッド", "Gold_Ingot.dds" );
	AddItem( enItem_Diamond, L"ダイアモンド", "Diamond.dds" );

	texDir.assign( "Resource/spriteData/tools" );

	//木のツール類
	AddItem( enItem_Wood_Sword, L"木の剣", "Wood_Sword.dds", 1 );
	AddItem( enItem_Wood_Pickaxe, L"木のピッケル", "Wood_Pickaxe.dds", 1 );
	AddItem( enItem_Wood_Shovel, L"木のシャベル", "Wood_Shovel.dds", 1 );
	AddItem( enItem_Wood_Axe, L"木のオノ", "Wood_Axe.dds", 1 );
	AddItem( enItem_Wood_Hoe, L"木のクワ", "Wood_Hoe.dds", 1 );

	//石のツール類
	AddItem( enItem_Stone_Sword, L"石の剣", "Stone_Sword.dds", 1 );
	AddItem( enItem_Stone_Pickaxe, L"石のピッケル", "Stone_Pickaxe.dds", 1 );
	AddItem( enItem_Stone_Shovel, L"石のシャベル", "Stone_Shovel.dds", 1 );
	AddItem( enItem_Stone_Axe, L"石のオノ", "Stone_Axe.dds", 1 );
	AddItem( enItem_Stone_Hoe, L"石のクワ", "Stone_Hoe.dds", 1 );

	//鉄のツール類
	AddItem( enItem_Iron_Sword, L"鉄の剣", "Iron_Sword.dds", 1 );
	AddItem( enItem_Iron_Pickaxe, L"鉄のピッケル", "Iron_Pickaxe.dds", 1 );
	AddItem( enItem_Iron_Shovel, L"鉄のシャベル", "Iron_Shovel.dds", 1 );
	AddItem( enItem_Iron_Axe, L"鉄のオノ", "Iron_Axe.dds", 1 );
	AddItem( enItem_Iron_Hoe, L"鉄のクワ", "Iron_Hoe.dds", 1 );

	//金のツール類
	AddItem( enItem_Gold_Sword, L"金の剣", "Gold_Sword.dds", 1 );
	AddItem( enItem_Gold_Pickaxe, L"金のピッケル", "Gold_Pickaxe.dds", 1 );
	AddItem( enItem_Gold_Shovel, L"金のシャベル", "Gold_Shovel.dds", 1 );
	AddItem( enItem_Gold_Axe, L"金のオノ", "Gold_Axe.dds", 1 );
	AddItem( enItem_Gold_Hoe, L"金のクワ", "Gold_Hoe.dds", 1 );

	//ダイヤのツール類
	AddItem( enItem_Diamond_Sword, L"ダイヤの剣", "Diamond_Sword.dds", 1 );
	AddItem( enItem_Diamond_Pickaxe, L"ダイヤのピッケル", "Diamond_Pickaxe.dds", 1 );
	AddItem( enItem_Diamond_Shovel, L"ダイヤのシャベル", "Diamond_Shovel.dds", 1 );
	AddItem( enItem_Diamond_Axe, L"ダイヤのオノ", "Diamond_Axe.dds", 1 );
	AddItem( enItem_Diamond_Hoe, L"ダイヤのクワ", "Diamond_Hoe.dds", 1 );

	for( auto& pa : m_nameMap ){
		OutputDebugStringA( pa.first.c_str() );
		OutputDebugStringA( "\n" );
	}
}
