//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

//! @brief Block を生成するクラス。
//! @author Takayama
class BlockFactory{
public:

	//! @brief json情報を使用して初期化。この関数を始めに呼ぶこと。
	static void Init(std::filesystem::path jsonFolder);

	//! @brief モデルパスを取得。 Item クラスが使用する。
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief ブロックを生成する。
	//! @param blockType ブロックの種類。
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );
};

