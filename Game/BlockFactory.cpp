#include "stdafx.h"
#include "BlockFactory.h"

#include "CraftingTable.h"

#include "BlockRenderingLightParameter.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};
static int BLOCK_HP_ARRAY[enCube_Num]{};

SkinModelEffectShader BlockFactory::m_s_ps;
int BlockFactory::m_instanceMax = -1;

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
	FILE_PATH_ARRAY[enCube_Glass] = L"Resource/modelData/GlassBlock.tkm";

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
	BLOCK_HP_ARRAY[enCube_Glass] = 4;

	//シェーダー読み込み
	D3D_SHADER_MACRO macros[] = {
		"INSTANCING", "1",
		NULL, NULL
	};
	m_s_ps.Load("Preset/shader/_u_blockShader.fx", "PSMain_McBlockRenderGBuffer", Shader::EnType::PS, "INSTANCING", macros);

	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( auto type : FILE_PATH_ARRAY ){
		if( !type )continue;

		GameObj::InstancingModel* instanceModel = mngr.Load( instanceMax, type );
		
		instanceModel->SetIsFrustumCulling( true );

		//ライティング用IInstanceDataを設定
		instanceModel->AddIInstanceData(L"BlockRenderingLightParameter", std::make_unique<BlockRenderingLightParameter>(instanceMax));
		//シェーダー設定
		instanceModel->GetModelRender().GetSkinModel().FindMaterialSetting(
			[&](MaterialSetting* mat) {
				mat->SetPS(&m_s_ps);
			}
		);
	}

	m_instanceMax = instanceMax;
}

void BlockFactory::FindBlockModel(std::function<void(GameObj::InstancingModel*)> func) {
	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();
	for (auto type : FILE_PATH_ARRAY) {
		if (!type)continue;
		GameObj::InstancingModel* instanceModel = mngr.Load(m_instanceMax, type);

		//処理実行
		func(instanceModel);
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
