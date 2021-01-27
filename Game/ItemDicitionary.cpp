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
	AddBlock( enCube_TorchBlock, L"�����u���b�N" );
	AddBlock( enCube_Torch, L"����");
	AddBlock( enCube_GrassHalf, L"���n�[�t" );

	//�A�Z�b�g���������t�H���_�B
	std::filesystem::path texDir;
	std::filesystem::path modelDir;

	//�X�^�b�N����B
	int stackLimit;

	//�A�C�e����o�^����֐��I�u�W�F�N�g�B
	auto AddItemWithStrID =
		[&]( EnItem id, const char* stringId, const wchar_t * name, std::string resource, bool haveModel = false){

		std::filesystem::path modelPath = "Resource/modelData/tools/Wood_Sword.tkm";
		if( haveModel ){
			modelPath.assign(modelDir / ( resource + ".tkm" ));
		}
		m_array[id] = Item( id, name, stackLimit, texDir / (resource + ".dds"), modelPath);
		m_nameMap.emplace( stringId, &( m_array[id] ) );
	};

	//enum��������ƍ��킹�ēo�^���邽�߂̃}�N���B
#define AddItem(id, name, ...) AddItemWithStrID(id, #id, name, __VA_ARGS__);

	//�u���b�N�ł͂Ȃ��A�C�e����o�^�B

	//�c�[���ł͂Ȃ��A�C�e���ށB
	stackLimit = 64;
	texDir.assign( "Resource/spriteData/items" );

	AddItem( enItem_Rod, L"�_", "Rod" );
	AddItem( enItem_Coal, L"�ΒY", "Coal" );
	AddItem( enItem_Charcoal, L"�ؒY", "Charcoal" );
	AddItem( enItem_Iron_Ingot, L"�S�C���S�b�h", "Iron_Ingot" );
	AddItem( enItem_Gold_Ingot, L"���C���S�b�h", "Gold_Ingot" );
	AddItem( enItem_Diamond, L"�_�C�A�����h", "Diamond" );

	//�c�[����
	stackLimit = 1;
	texDir.assign( "Resource/spriteData/tools" );
	modelDir.assign( "Resource/modelData/tools" );

	//�؂̃c�[����
	AddItem( enItem_Wood_Sword, L"�؂̌�", "Wood_Sword", true);
	AddItem( enItem_Wood_Pickaxe, L"�؂̃s�b�P��", "Wood_Pickaxe" );
	AddItem( enItem_Wood_Shovel, L"�؂̃V���x��", "Wood_Shovel" );
	AddItem( enItem_Wood_Axe, L"�؂̃I�m", "Wood_Axe");
	AddItem( enItem_Wood_Hoe, L"�؂̃N��", "Wood_Hoe");

	//�΂̃c�[����
	AddItem( enItem_Stone_Sword, L"�΂̌�", "Stone_Sword", true );
	AddItem( enItem_Stone_Pickaxe, L"�΂̃s�b�P��", "Stone_Pickaxe");
	AddItem( enItem_Stone_Shovel, L"�΂̃V���x��", "Stone_Shovel");
	AddItem( enItem_Stone_Axe, L"�΂̃I�m", "Stone_Axe");
	AddItem( enItem_Stone_Hoe, L"�΂̃N��", "Stone_Hoe");

	//�S�̃c�[����
	AddItem( enItem_Iron_Sword, L"�S�̌�", "Iron_Sword", true );
	AddItem( enItem_Iron_Pickaxe, L"�S�̃s�b�P��", "Iron_Pickaxe" );
	AddItem( enItem_Iron_Shovel, L"�S�̃V���x��", "Iron_Shovel");
	AddItem( enItem_Iron_Axe, L"�S�̃I�m", "Iron_Axe");
	AddItem( enItem_Iron_Hoe, L"�S�̃N��", "Iron_Hoe");

	//���̃c�[����
	AddItem( enItem_Gold_Sword, L"���̌�", "Gold_Sword", true );
	AddItem( enItem_Gold_Pickaxe, L"���̃s�b�P��", "Gold_Pickaxe");
	AddItem( enItem_Gold_Shovel, L"���̃V���x��", "Gold_Shovel");
	AddItem( enItem_Gold_Axe, L"���̃I�m", "Gold_Axe");
	AddItem( enItem_Gold_Hoe, L"���̃N��", "Gold_Hoe");

	//�_�C���̃c�[����
	AddItem( enItem_Diamond_Sword, L"�_�C���̌�", "Diamond_Sword", true );
	AddItem( enItem_Diamond_Pickaxe, L"�_�C���̃s�b�P��", "Diamond_Pickaxe");
	AddItem( enItem_Diamond_Shovel, L"�_�C���̃V���x��", "Diamond_Shovel");
	AddItem( enItem_Diamond_Axe, L"�_�C���̃I�m", "Diamond_Axe" );
	AddItem( enItem_Diamond_Hoe, L"�_�C���̃N��", "Diamond_Hoe" );
}
