//! @file
#pragma once
#include "../BlockType.h"

//! @brief ブロックの情報とモデルレンダーを持つクラス。
//! @details 生成には BlockFactory を使用してほしい。
//! @author Takayama
class Block{
public:
	Block();
	~Block();

	//! @brief ポジションをセット。
	//! @details Worldは1ブロック1単位で座標を運用しているため、モデルにはブロックの幅を乗算した値を設定している。

	void SetPos( int x, int y, int z );

	GameObj::CInstancingModelRender& GetModel(){
		return m_model;
	}

	//! @brief ブロックの種類を設定。
	//! @details この関数はモデルを変更しない。
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	//! @brief ブロックの種類を設定。
	EnCube GetBlockType() const
	{
		return m_state;
	}

	//! @brief モデルの描画をするかどうかを設定。
	//! @details 埋まっていて見えないブロックの描画を止めるために使用される。
	void SetIsDraw(bool isDraw){
		m_model.SetIsDraw( isDraw );
	}

	bool IsCollisionEnabled() const{
		return m_collision.operator bool();
	}

	void EnableCollision();

	void DisableCollision(){
		m_collision.reset();
	}

	//! @brief ブロックの幅、奥行き、高さ。
	static constexpr float WIDTH = 140;
private:
	GameObj::CInstancingModelRender m_model;
	//! @brief ブロックの種類。
	EnCube m_state = enCube_None;

	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;
};

