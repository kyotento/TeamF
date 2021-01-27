#include "stdafx.h"
#include "BlockFactory.h"

#include "CraftingTable.h"

#include "BlockRenderingLightParameter.h"

namespace {
	constexpr float BLOCK_SIZE_HALF = Block::WIDTH * 0.5f;
}

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};
static int BLOCK_HP_ARRAY[enCube_Num]{};

SkinModelEffectShader BlockFactory::m_s_ps;
int BlockFactory::m_instanceMax = -1;
AABB BlockFactory::BLOCK_AABB_ARRAY[enCube_Num];
char BlockFactory::BLOCK_LIGHT_ARRAY[enCube_Num];
bool BlockFactory::BLOCK_OPACITY[enCube_Num];

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
	FILE_PATH_ARRAY[enCube_TorchBlock] = L"Resource/modelData/TorchBlock.tkm";
	FILE_PATH_ARRAY[enCube_Torch] = L"Resource/modelData/Torch.tkm";
	FILE_PATH_ARRAY[enCube_GrassHalf] = L"Resource/modelData/GrassHalf.tkm";	

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
	BLOCK_HP_ARRAY[enCube_TorchBlock] = 1;
	BLOCK_HP_ARRAY[enCube_Torch] = 1;
	BLOCK_HP_ARRAY[enCube_GrassHalf] = 4;

	for (int i = 0; i < enCube_Num; i++) {
		BLOCK_AABB_ARRAY[i].min = { -BLOCK_SIZE_HALF , 0.0f, -BLOCK_SIZE_HALF };
		BLOCK_AABB_ARRAY[i].max = { BLOCK_SIZE_HALF , BLOCK_SIZE_HALF * 2.0f, BLOCK_SIZE_HALF };
		BLOCK_LIGHT_ARRAY[i] = 0;
		BLOCK_OPACITY[i] = true;
	}
	//松明ブロック
	BLOCK_LIGHT_ARRAY[enCube_TorchBlock] = 14;
	//松明
	BLOCK_LIGHT_ARRAY[enCube_Torch] = 14;
	BLOCK_OPACITY[enCube_Torch] = false;
	BLOCK_AABB_ARRAY[enCube_Torch].min.x *= 0.5f;
	BLOCK_AABB_ARRAY[enCube_Torch].min.x /= 7.0f;
	BLOCK_AABB_ARRAY[enCube_Torch].min.z *= 0.5f;
	BLOCK_AABB_ARRAY[enCube_Torch].min.z /= 7.0f;
	BLOCK_AABB_ARRAY[enCube_Torch].max *= 0.5f;
	BLOCK_AABB_ARRAY[enCube_Torch].max.x /= 7.0f;
	BLOCK_AABB_ARRAY[enCube_Torch].max.z /= 7.0f;
	//草ハーフ
	BLOCK_OPACITY[enCube_GrassHalf] = false;
	BLOCK_AABB_ARRAY[enCube_GrassHalf].max.y *= 0.5f;

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
