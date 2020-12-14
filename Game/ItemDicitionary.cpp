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

	//enum��������ƍ��킹�ēo�^���邽�߂̃}�N���B
#define AddBlock(id, name) AddBlockWithStrID(id, #id, name);

	//�u���b�N����A�C�e���������A�C�e����o�^�B
	AddBlock( enCube_Grass, L"��" );
	AddBlock( enCube_Soil, L"�y" );
	AddBlock( enCube_Stone, L"��" );
	AddBlock( enCube_CobbleStone, L"�ې�" );
	AddBlock( enCube_OakLeaf, L"�t" );
	AddBlock( enCube_OakLog, L"�I�[�N�̌���" );
	AddBlock( enCube_OakWood, L"�I�[�N�̖؍�" );
	AddBlock( enCube_CoalOre, L"�ΒY�z��" );
	AddBlock( enCube_IronOre, L"�S�z��" );
	AddBlock( enCube_GoldOre, L"���z��" );
	AddBlock( enCube_CraftingTable, L"��Ƒ�" );

	//�A�C�e����o�^����֐��I�u�W�F�N�g�B
	std::filesystem::path texDir;

	auto AddItemWithStrID =
		[&]( EnItem id, const char* stringId, const wchar_t * name,
			 const std::filesystem::path & resource, int stackLimit = 64 ){

		m_array[id] = Item( id, name, stackLimit, texDir / resource );
		m_nameMap.emplace( stringId, &( m_array[id] ) );
	};

	//enum��������ƍ��킹�ēo�^���邽�߂̃}�N���B
#define AddItem(id, name, ...) AddItemWithStrID(id, #id, name, __VA_ARGS__);

	//�u���b�N�ł͂Ȃ��A�C�e����o�^�B
	texDir.assign( "Resource/spriteData/items" );

	AddItem( enItem_Rod, L"�_", "Rod.dds" );
	AddItem( enItem_Coal, L"�ΒY", "Coal.dds" );
	AddItem( enItem_Charcoal, L"�ؒY", "Charcoal.dds" );
	AddItem( enItem_Iron_Ingot, L"�S�C���S�b�h", "Iron_Ingot.dds" );
	AddItem( enItem_Gold_Ingot, L"���C���S�b�h", "Gold_Ingot.dds" );
	AddItem( enItem_Diamond, L"�_�C�A�����h", "Diamond.dds" );

	texDir.assign( "Resource/spriteData/tools" );

	//�؂̃c�[����
	AddItem( enItem_Wood_Sword, L"�؂̌�", "Wood_Sword.dds", 1 );
	AddItem( enItem_Wood_Pickaxe, L"�؂̃s�b�P��", "Wood_Pickaxe.dds", 1 );
	AddItem( enItem_Wood_Shovel, L"�؂̃V���x��", "Wood_Shovel.dds", 1 );
	AddItem( enItem_Wood_Axe, L"�؂̃I�m", "Wood_Axe.dds", 1 );
	AddItem( enItem_Wood_Hoe, L"�؂̃N��", "Wood_Hoe.dds", 1 );

	//�΂̃c�[����
	AddItem( enItem_Stone_Sword, L"�΂̌�", "Stone_Sword.dds", 1 );
	AddItem( enItem_Stone_Pickaxe, L"�΂̃s�b�P��", "Stone_Pickaxe.dds", 1 );
	AddItem( enItem_Stone_Shovel, L"�΂̃V���x��", "Stone_Shovel.dds", 1 );
	AddItem( enItem_Stone_Axe, L"�΂̃I�m", "Stone_Axe.dds", 1 );
	AddItem( enItem_Stone_Hoe, L"�΂̃N��", "Stone_Hoe.dds", 1 );

	//�S�̃c�[����
	AddItem( enItem_Iron_Sword, L"�S�̌�", "Iron_Sword.dds", 1 );
	AddItem( enItem_Iron_Pickaxe, L"�S�̃s�b�P��", "Iron_Pickaxe.dds", 1 );
	AddItem( enItem_Iron_Shovel, L"�S�̃V���x��", "Iron_Shovel.dds", 1 );
	AddItem( enItem_Iron_Axe, L"�S�̃I�m", "Iron_Axe.dds", 1 );
	AddItem( enItem_Iron_Hoe, L"�S�̃N��", "Iron_Hoe.dds", 1 );

	//���̃c�[����
	AddItem( enItem_Gold_Sword, L"���̌�", "Gold_Sword.dds", 1 );
	AddItem( enItem_Gold_Pickaxe, L"���̃s�b�P��", "Gold_Pickaxe.dds", 1 );
	AddItem( enItem_Gold_Shovel, L"���̃V���x��", "Gold_Shovel.dds", 1 );
	AddItem( enItem_Gold_Axe, L"���̃I�m", "Gold_Axe.dds", 1 );
	AddItem( enItem_Gold_Hoe, L"���̃N��", "Gold_Hoe.dds", 1 );

	//�_�C���̃c�[����
	AddItem( enItem_Diamond_Sword, L"�_�C���̌�", "Diamond_Sword.dds", 1 );
	AddItem( enItem_Diamond_Pickaxe, L"�_�C���̃s�b�P��", "Diamond_Pickaxe.dds", 1 );
	AddItem( enItem_Diamond_Shovel, L"�_�C���̃V���x��", "Diamond_Shovel.dds", 1 );
	AddItem( enItem_Diamond_Axe, L"�_�C���̃I�m", "Diamond_Axe.dds", 1 );
	AddItem( enItem_Diamond_Hoe, L"�_�C���̃N��", "Diamond_Hoe.dds", 1 );

	for( auto& pa : m_nameMap ){
		OutputDebugStringA( pa.first.c_str() );
		OutputDebugStringA( "\n" );
	}
}
