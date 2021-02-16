#include "stdafx.h"
#include "BlockFactory.h"
#include "BlockInfoDictionary.h"
#include "Chunk.h"
#include "RandomMapMaker.h"

#include "CraftingTable.h"
#include "WorthOfGod.h"
#include "Door.h"
#include "Bed.h"
#include "TNT.h"
#include "Wood.h"

#include "Chest.h"
#include "Furnace.h"

#include "BlockRenderingLightParameter.h"

namespace{
	//�u���b�N�����i�[����N���X�B
	BlockInfoDictionary st_blockInfo;

	constexpr float BLOCK_SIZE_HALF = Block::WIDTH * 0.5f;
}

SkinModelEffectShader BlockFactory::m_s_ps;

int BlockFactory::m_instanceMax = -1;

void BlockFactory::Init( std::filesystem::path jsonFolder){
	//�u���b�N���̃��[�h�B
	st_blockInfo.Load( jsonFolder );

	//�V�F�[�_�[�ǂݍ���
	D3D_SHADER_MACRO macros[] = {
		"INSTANCING", "1",
		NULL, NULL
	};
	m_s_ps.Load( "Preset/shader/_u_blockShader.fx", "PSMain_McBlockRenderGBuffer", Shader::EnType::PS, "INSTANCING", macros );

	//�C���X�^���V���O���f���̃��[�h
	int loadEdge = 1 * 2 * Chunk::WIDTH;
	m_instanceMax = loadEdge * loadEdge * ( int( RandomMapMaker::m_maxHeight ) + 1 );

	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( const auto& entry : GetBlockMap() ){
		GameObj::InstancingModel* instanceModel = mngr.Load( m_instanceMax, entry.second.modelPath.c_str() );
		instanceModel->SetIsFrustumCulling( true );
		instanceModel->SetIsFrustumCulling( true );

		//���C�e�B���O�pIInstanceData��ݒ�
		instanceModel->AddIInstanceData( L"BlockRenderingLightParameter", std::make_unique<BlockRenderingLightParameter>( m_instanceMax ) );
		//�V�F�[�_�[�ݒ�
		instanceModel->GetModelRender().GetSkinModel().FindMaterialSetting(
			[&]( MaterialSetting* mat ){
				mat->SetPS( &m_s_ps );
			}
		);
	}
}

void BlockFactory::FindBlockModel( std::function<void( GameObj::InstancingModel* )> func ){
	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();
	for( auto entry : GetBlockMap() ){
		GameObj::InstancingModel* instanceModel = mngr.Load( m_instanceMax, entry.second.modelPath.c_str() );

		//�������s
		func( instanceModel );
	}
}

const wchar_t * BlockFactory::GetModelPath( EnCube blockType ){
	return st_blockInfo.GetInfo(blockType).modelPath.c_str();
}

std::unique_ptr<Block> BlockFactory::CreateBlock(EnCube blockType, Block::enMuki muki){
	std::unique_ptr<Block> block;

	//�����_������
	if (muki == Block::enMuki::enNum) {
		muki = (Block::enMuki)(CMath::RandomInt() % 4);
	}

	//�N���b�N���ɃA�N�V�������N�����u���b�N�Ȃǂ͕ʂ̃N���X�B
	switch( blockType ){
	case enCube_CraftingTable:
		block = std::make_unique<CraftingTable>();
		break;
	case enCube_WoGBlock:
		block = std::make_unique<WorthOfGod>();
		break;
	case enCube_DoorUp:
	case enCube_DoorDown:
		block = std::make_unique<Door>( blockType == enCube_DoorUp ? true : false );
		break;
	case enCube_Chest:
		block = std::make_unique<Chest>();
		break;
	case enCube_BedHead:
	case enCube_BedLeg:
		block = std::make_unique<Bed>(blockType == enCube_BedHead ? true : false, Block::GetMukiDir(muki));
		break;
	case enCube_Furnace:
		block = std::make_unique<Furnace>();
		break;
	case enCube_TNT:
		block = std::make_unique<TNT>();
		break;
	case enCube_OakLeaf:
		block = std::make_unique<Leaf>(enCube_OakLog, enCube_OakLeaf);
		break;
	default:
		block = std::make_unique<Block>();
		break;
	}

	//���ʏ����g���ď������B
	const BlockInfo& bInfo = st_blockInfo.GetInfo( blockType );
	block->Init( &bInfo, muki );

	return std::move( block );
}

const std::unordered_map<EnCube, BlockInfo>& BlockFactory::GetBlockMap(){
	return st_blockInfo.GetMap();
}
