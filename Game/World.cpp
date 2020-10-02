#include "stdafx.h"
#include "World.h"

Block * World::GetBlock( int x, int y, int z ){
	try{

		Chunk* chunk = GetChunkFromWorldPos( x, z );
		x = Chunk::CalcInChunkCoord( x );
		z = Chunk::CalcInChunkCoord( z );
		return chunk->GetBlock( x, y, z );

	} catch( std::out_of_range e ){
		return nullptr;
	}
}

void World::SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
	Chunk* chunk;

	try{
		chunk = GetChunkFromWorldPos( x, z );
	} catch( std::out_of_range e ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	Block* b = block.get();

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );
	chunk->SetBlock( x, y, z, std::move(block) );

	auto p = b->GetModel().GetPos();

	if( y - p.y > 1 ){
		int i = 0;
	}
}

Chunk* World::GetChunkFromWorldPos( int x, int z ){
	int chunkX = CalcChunkCoord(x);
	int chunkZ = CalcChunkCoord(z);
	return &m_chunkMap.at( chunkX ).at( chunkZ );
}

Chunk * World::CreateChunkFromWorldPos( int x, int z ){
	int chunkX = CalcChunkCoord( x );
	int chunkZ = CalcChunkCoord( z );
	Chunk* chunk = &m_chunkMap[chunkX][chunkZ];
	chunk->SetChunkPos( chunkX, chunkZ );
	return chunk;
}
