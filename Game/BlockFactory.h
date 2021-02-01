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

	//! @brief 各ブロックモデルを検索して処理を実行。
	static void FindBlockModel(std::function<void(GameObj::InstancingModel*)> func);

	//! @brief モデルパスを取得。 Item クラスが使用する。
	static const wchar_t* GetModelPath( EnCube blockType );

	//! @brief ブロックのＡＡＢＢを取得する。
	static const AABB& GetAABB(EnCube blockType, int index) {
		return BLOCK_AABB_ARRAY[blockType][index];
	}
	//! @brief ブロックのＡＡＢＢの数を取得する。
	static int GetAABBNum(EnCube blockType) {		
		return BLOCK_AABB_NUM[blockType];
	}
	//! @brief ブロックの明るさを取得する。
	static char GetLight(EnCube blockType) {
		return BLOCK_LIGHT_ARRAY[blockType];
	}
	//! @brief ブロックが不透明かどうかを取得する。
	static bool GetIsOpacity(EnCube blockType) {
		return blockType == enCube_None ? false : BLOCK_OPACITY[blockType];
	}

	//! @brief ブロックを生成する。
	//! @param blockType ブロックの種類。
	static std::unique_ptr<Block> CreateBlock( EnCube blockType );		

private:
	//ブロック描画シェーダー
	static SkinModelEffectShader m_s_ps;

	//インスタンス数
	static int m_instanceMax;

	//ブロックのAABB
	static AABB BLOCK_AABB_ARRAY[enCube_Num][Block::BLOCK_AABB_MAXNUM];
	static int  BLOCK_AABB_NUM[enCube_Num];
	//ブロックの明るさ
	static char BLOCK_LIGHT_ARRAY[enCube_Num];
	//ブロックが不透明か
	static bool BLOCK_OPACITY[enCube_Num];
};

