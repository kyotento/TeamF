#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"

class World{
public:
	void Generate(){
		m_mapmMaker.Init(this);
	}

	Block* GetBlock( const CVector3& pos ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock( int x, int y, int z );

	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block );

	Chunk* GetChunkFromWorldPos( int x, int z );

	Chunk* CreateChunkFromWorldPos( int x, int z );

	//ワールド生成後に埋まっているブロックを非表示にする。
	void GenerateEndCulling();

	//チャンク座標を計算
	static int CalcChunkCoord( int num ){
		if( num < 0 )num -= Chunk::WIDTH - 1;
		return num / Chunk::WIDTH;
	}

private:
	RandomMapMaker m_mapmMaker;
	std::map<int, std::map<int, Chunk>> m_chunkMap;
};

