#include "stdafx.h"
#include "BlockFactory.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num - 1]{};

void BlockFactory::LoadInstancingModels( int instanceMax ){
	FILE_PATH_ARRAY[enCube_Grass] = L"Resource/modelData/GrassBlock.cmo";
	FILE_PATH_ARRAY[enCube_Soil] = L"Resource/modelData/soilBlock.cmo";
	FILE_PATH_ARRAY[enCube_Stone] = L"Resource/modelData/stoneBlock.cmo";

	auto& mngr = GameObj::CInstancingModelRender::GetInstancingModelManager();

	for( auto type : FILE_PATH_ARRAY ){
		if( !type )continue;

		GameObj::InstancingModel* instanceModel = mngr.Load( instanceMax, type );
		
		instanceModel->SetIsFrustumCulling( true );
	}
}

std::unique_ptr<Block> BlockFactory::CreateBlock( EnCube blockType ){
	auto block = std::make_unique<Block>();
	//instanceMaxはすでにモデルがロードされている場合は使われないので値が何でも関係ない。
	block->GetModel().Init( 0, FILE_PATH_ARRAY[blockType] );
	//block->GetModel().SetScale( CVector3::One() * 0.0075f );
	block->SetBlockType( blockType );

	return std::move( block );
}
