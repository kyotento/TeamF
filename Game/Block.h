#pragma once

class Block{
public:
	Block();

	void SetPos( int x, int y, int z ){
		m_model.SetPos( CVector3( x + 0.5f, y + 0.5f, z + 0.5f ) );
	}

private:
	GameObj::CInstancingModelRender m_model;
};

