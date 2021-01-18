#include "stdafx.h"
#include "BlockFactory.h"

#include "CraftingTable.h"

#include "BlockRenderingLightParameter.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

SkinModelEffectShader BlockFactory::m_s_ps;

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
	FILE_PATH_ARRAY[enCube_CraftingTable] = L"Resource/modelData/craftingTable.tkm";

	//�V�F�[�_�[�ǂݍ���
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

		//���C�e�B���O�pIInstanceData��ݒ�
		instanceModel->AddIInstanceData(L"BlockRenderingLightParameter", std::make_unique<BlockRenderingLightParameter>(instanceMax));
		//�V�F�[�_�[�ݒ�
		instanceModel->GetModelRender().GetSkinModel().FindMaterialSetting(
			[&](MaterialSetting* mat) {
				mat->SetPS(&m_s_ps);
			}
		);
	}
}

const wchar_t * BlockFactory::GetModelPath( EnCube blockType ){
	return FILE_PATH_ARRAY[blockType];
}

std::unique_ptr<Block> BlockFactory::CreateBlock( EnCube blockType ){
	std::unique_ptr<Block> block;

	//�N���b�N���ɃA�N�V�������N�����u���b�N�Ȃǂ͕ʂ̃N���X�B
	if( blockType == enCube_CraftingTable ){
		block = std::make_unique<CraftingTable>();
	} else{
		block = std::make_unique<Block>();
	}
		
	block->InitModel(FILE_PATH_ARRAY[blockType]);
	block->SetBlockType( blockType );

	return std::move( block );
}