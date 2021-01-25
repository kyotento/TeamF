#include "stdafx.h"
#include "BlockFactory.h"

#include "CraftingTable.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};
static int BLOCK_HP_ARRAY[enCube_Num]{};

void BlockFactory::LoadInstancingModels( int instanceMax ){

	FILE_PATH_ARRAY[enCube_Grass] = L"Resource/modelData/GrassBlock.tkm";
	FILE_PATH_ARRAY[enCube_Soil] = L"Resource/modelData/soilBlock.tkm";
	FILE_PATH_ARRAY[enCube_Stone] = L"Resource/modelData/stoneBlock.tkm";
	FILE_PATH_ARRAY[enCube_CobbleStone] = L"Resource/modelData/cobbleStone.tkm";
	FILE_PATH_ARRAY[enCube_OakLog] = L"Resource/modelData/oakLog.tkm";
	FILE_PATH_ARRAY[enCube_OakWood] = L"Resource/modelData/oakWood.tkm";
	FILE_PATH_ARRAY[enCube_OakLeaf] = L"Resource/modelData/leafBlock.tkm";
	FILE_PATH_ARRAY[enCube_CoalOre] = L"Resource/modelData/coalOre.tkm";
	FILE_PATH_ARRAY[enCube_IronOre] = L"Resource/modelData/ironOre.tkm";
	FILE_PATH_ARRAY[enCube_GoldOre] = L"Resource/modelData/goldOre.tkm";
	FILE_PATH_ARRAY[enCube_Bedrock] = L"Resource/modelData/GrassBlock.tkm";
	FILE_PATH_ARRAY[enCube_CraftingTable] = L"Resource/modelData/craftingTable.tkm";

	BLOCK_HP_ARRAY[enCube_Grass] = 4;
	BLOCK_HP_ARRAY[enCube_Soil] = 4;
	BLOCK_HP_ARRAY[enCube_Stone] = 16;
	BLOCK_HP_ARRAY[enCube_CobbleStone] = 16;
	BLOCK_HP_ARRAY[enCube_OakLog] = 8;
	BLOCK_HP_ARRAY[enCube_OakWood] = 12;
	BLOCK_HP_ARRAY[enCube_OakLeaf] = 2;
	BLOCK_HP_ARRAY[enCube_CoalOre] = 20;
	BLOCK_HP_ARRAY[enCube_IronOre] = 28;
	BLOCK_HP_ARRAY[enCube_GoldOre] = 36;
	BLOCK_HP_ARRAY[enCube_Bedrock] = 40;
	BLOCK_HP_ARRAY[enCube_CraftingTable] = 4;


	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( auto type : FILE_PATH_ARRAY ){
		if( !type )continue;

		GameObj::InstancingModel* instanceModel = mngr.Load( instanceMax, type );
		
		instanceModel->SetIsFrustumCulling( true );
	}
}

const wchar_t * BlockFactory::GetModelPath( EnCube blockType ){
	return FILE_PATH_ARRAY[blockType];
}

std::unique_ptr<Block> BlockFactory::CreateBlock( EnCube blockType ){
	std::unique_ptr<Block> block;

	//クリック時にアクションを起こすブロックなどは別のクラス。
	if( blockType == enCube_CraftingTable ){
		block = std::make_unique<CraftingTable>();
	} else{
		block = std::make_unique<Block>();
	}
		
	block->InitModel(FILE_PATH_ARRAY[blockType]);
	block->SetBlockType( blockType );
	block->SetHP(BLOCK_HP_ARRAY[blockType]);

	return std::move( block );
}