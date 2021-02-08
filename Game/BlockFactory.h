//! @file
#pragma once
#include "BlockType.h"
#include "Block.h"

class BlockInfo;

//! @brief Block を生成するクラス。
//! @author Takayama
class BlockFactory{
public:

	//! @brief json情報を使用して初期化。この関数を始めに呼ぶこと。
	static void Init(std::filesystem::path jsonFolder);

	//! @brief モデルパスを取得。
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief 各ブロックモデルを検索して処理を実行。
	static void FindBlockModel( std::function<void( GameObj::InstancingModel* )> func );

	//! @brief ブロックを生成する。
	//! @param blockType ブロックの種類。
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );

	static const std::unordered_map<EnCube, BlockInfo>& GetBlockMap();

private:
	//ブロック描画シェーダー
	static SkinModelEffectShader m_s_ps;

	//インスタンス数
	static int m_instanceMax;
};

