#include "stdafx.h"
#include "BlockFactory.h"
#include "BlockInfoDictionary.h"
#include "Chunk.h"
#include "RandomMapMaker.h"

#include "CraftingTable.h"

namespace{
	//ブロック情報を格納するクラス。
	BlockInfoDictionary st_blockInfo;
}

void BlockFactory::Init( std::filesystem::path jsonFolder){

	//ブロック情報のロード。
	st_blockInfo.Load( jsonFolder );

	//インスタンシングモデルのロード
	int loadEdge = 1 * 2 * Chunk::WIDTH;
	int instanceMax = loadEdge * loadEdge * ( int( RandomMapMaker::m_maxHeight ) + 1 );

	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( const auto& entry : GetBlockMap() ){
		GameObj::InstancingModel* instanceModel = mngr.Load( instanceMax, entry.second.modelPath.c_str() );
	}
}

const wchar_t * BlockFactory::GetModelPath( EnCube blockType ){
	return st_blockInfo.GetInfo(blockType).modelPath.c_str();
}

std::unique_ptr<Block> BlockFactory::CreateBlock( EnCube blockType ){
	std::unique_ptr<Block> block;

	//クリック時にアクションを起こすブロックなどは別のクラス。
	if( blockType == enCube_CraftingTable ){
		block = std::make_unique<CraftingTable>();
	} else{
		block = std::make_unique<Block>();
	}

	BlockInfo bInfo = st_blockInfo.GetInfo( blockType );
		
	block->InitModel(bInfo.modelPath.c_str());
	block->SetBlockType( blockType );
	block->SetHP(bInfo.hp);

	return std::move( block );
}

const std::unordered_map<EnCube, BlockInfo>& BlockFactory::GetBlockMap(){
	return st_blockInfo.GetMap();
}
