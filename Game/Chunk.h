#pragma once
#include "Block.h"

class Chunk{
public:
	void SetChunkPos( int x, int z ){
		m_chunkX = x; m_chunkZ = z;
	}

	//チャンク内の座標でブロックを取得
	Block* GetBlock( int x, int y, int z ){
		return m_blockArray[x][y][z].get();
	}
	Block* GetBlock( const CVector3& pos ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		return GetBlock( x, y, z );
	}

	//チャンク内の座標でブロックを設定
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block){
		m_blockArray[x][y][z] = std::move(block);
		m_blockArray[x][y][z]->SetPos( CalcWorldCoordX(x) * Block::WIDTH,
									   y * Block::WIDTH,
									   CalcWorldCoordZ(z) * Block::WIDTH );
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
	int CalcWorldCoordX( int x){
		return x + m_chunkX * Chunk::WIDTH;
	}
	int CalcWorldCoordZ( int z ){
		return z + m_chunkZ * Chunk::WIDTH;
	}

	int GetX() const{
		return m_chunkX;
	}

	int GetZ() const{
		return m_chunkZ;
	}
	const auto& Data() const{
		return m_blockArray;
	}

	auto& Data(){
		return m_blockArray;
	}

	static constexpr int WIDTH = 16;
	static constexpr int HEIGHT = 64;

private:
	int m_chunkX = 0;
	int m_chunkZ = 0;
	std::unique_ptr<Block> m_blockArray[WIDTH][HEIGHT][WIDTH];
};

