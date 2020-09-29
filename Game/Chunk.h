#pragma once
#include "Block.h"

class Chunk{
public:
	void SetChunkPos( int x, int z ){
		m_chunkX = x; m_chunkZ = z;
	}

	//チャンク内の座標でブロックを取得
	Block* GetBlock( int x, int y, int z ){
		return m_blockArray[z][y][x].get();
	}
	Block* GetBlock( const CVector3& pos ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		return GetBlock( x, y, z );
	}

	//チャンク内の座標でブロックを設定
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block){
		m_blockArray[z][x][y] = std::move(block);
		CalcWorldCoord( x, z );
		m_blockArray[z][x][y]->SetPos( x, y, z );
	}
	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}

	//チャンク内座標を計算
	static int CalcInChunkCoord( int coord ){
		if( coord < 0 ){
			coord -= Chunk::WIDTH - 1;
			return (Chunk::WIDTH - 1) + coord % Chunk::WIDTH;
		}
		return coord % Chunk::WIDTH;
	}

	//ワールド座標を計算
	void CalcWorldCoord( int& x , int& z){
		x += m_chunkX * Chunk::WIDTH;
		z += m_chunkZ * Chunk::WIDTH;
	}

	static constexpr int WIDTH = 16;
	static constexpr int HEIGHT = 40;
private:
	int m_chunkX = 0;
	int m_chunkZ = 0;
	std::unique_ptr<Block> m_blockArray[WIDTH][HEIGHT][WIDTH];
};

