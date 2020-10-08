#pragma once
#include "../BlockType.h"


class Block{
public:
	Block();

	void SetPos( int x, int y, int z ){
		m_model.SetPos( CVector3( x + 0.5f, y + 0.5f, z + 0.5f ) );
		m_collision.SetPosition(CVector3(x + 0.5f, y + 0.5f, z + 0.5f));
	}

	GameObj::CInstancingModelRender& GetModel(){
		return m_model;
	}
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	EnCube GetBlockType() const
	{
		return m_state;
	}
	void SetIsDraw(bool isDraw){
		m_model.SetIsDraw( isDraw );
		m_collision.SetEnable(isDraw);
	}
	SuicideObj::CCollisionObj& GetCollision() {
		return m_collision;
	}

	static constexpr float WIDTH = 140;

private:
	GameObj::CInstancingModelRender m_model;
	EnCube m_state = enCube_None;
	SuicideObj::CCollisionObj m_collision;
};

