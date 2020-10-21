//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

//! @brief Block を生成するクラス。
//! @author Takayama
class BlockFactory{
public:

	//! @brief モデルを読み込む。この関数を始めに呼ぶこと。
	//! @details インスタンシング描画に使用するモデルを読み込む。CreateBlockより前に呼ばれていなければならない。
	static void LoadInstancingModels(int instanceMax);

	//! @brief ブロックを生成する。
	//! @param blockType ブロックの種類。
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );

};

