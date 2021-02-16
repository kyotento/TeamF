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
	//ブロック情報を格納するクラス。
	BlockInfoDictionary st_blockInfo;

	constexpr float BLOCK_SIZE_HALF = Block::WIDTH * 0.5f;
}

SkinModelEffectShader BlockFactory::m_s_ps;

int BlockFactory::m_instanceMax = -1;

void BlockFactory::Init( std::filesystem::path jsonFolder){
	//ブロック情報のロード。
	st_blockInfo.Load( jsonFolder );

	//シェーダー読み込み
	D3D_SHADER_MACRO macros[] = {
		"INSTANCING", "1",
		NULL, NULL
	};
	m_s_ps.Load( "Preset/shader/_u_blockShader.fx", "PSMain_McBlockRenderGBuffer", Shader::EnType::PS, "INSTANCING", macros );

	//インスタンシングモデルのロード
	int loadEdge = 1 * 2 * Chunk::WIDTH;
	m_instanceMax = loadEdge * loadEdge * ( int( RandomMapMaker::m_maxHeight ) + 1 );

	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( const auto& entry : GetBlockMap() ){
		GameObj::InstancingModel* instanceModel = mngr.Load( m_instanceMax, entry.second.modelPath.c_str() );
		instanceModel->SetIsFrustumCulling( true );
		instanceModel->SetIsFrustumCulling( true );

		//ライティング用IInstanceDataを設定
		instanceModel->AddIInstanceData( L"BlockRenderingLightParameter", std::make_unique<BlockRenderingLightParameter>( m_instanceMax ) );
		//シェーダー設定
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

		//処理実行
		func( instanceModel );
	}
}

const wchar_t * BlockFactory::GetModelPath( EnCube blockType ){
	return st_blockInfo.GetInfo(blockType).modelPath.c_str();
}

std::unique_ptr<Block> BlockFactory::CreateBlock(EnCube blockType, Block::enMuki muki){
	std::unique_ptr<Block> block;

	//ランダム向き
	if (muki == Block::enMuki::enNum) {
		muki = (Block::enMuki)(CMath::RandomInt() % 4);
	}

	//クリック時にアクションを起こすブロックなどは別のクラス。
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

	//共通情報を使って初期化。
	const BlockInfo& bInfo = st_blockInfo.GetInfo( blockType );
	block->Init( &bInfo, muki );

	return std::move( block );
}

const std::unordered_map<EnCube, BlockInfo>& BlockFactory::GetBlockMap(){
	return st_blockInfo.GetMap();
}
