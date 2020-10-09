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

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );
	chunk->SetBlock( x, y, z, std::move( block ) );
}

Chunk* World::GetChunkFromWorldPos( int x, int z ){
	int chunkX = CalcChunkCoord( x );
	int chunkZ = CalcChunkCoord( z );
	return &m_chunkMap.at( chunkX ).at( chunkZ );
}

Chunk * World::CreateChunkFromWorldPos( int x, int z ){
	int chunkX = CalcChunkCoord( x );
	int chunkZ = CalcChunkCoord( z );
	Chunk* chunk = &m_chunkMap[chunkX][chunkZ];
	chunk->SetChunkPos( chunkX, chunkZ );
	return chunk;
}

void World::GenerateEndCulling(){
	struct intV3{
		int x, y, z;
	};

	intV3 vArray[]{
		{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
	};

	//全チャンクをループ
	for( auto& chunkMapZ : m_chunkMap ){
		for( auto& chunkPair : chunkMapZ.second ){
			auto& chunk = chunkPair.second;

			for( int y = 0; y < chunk.HEIGHT; y++ ){
				for( int x = 0; x < chunk.WIDTH; x++ ){
					for( int z = 0; z < chunk.WIDTH; z++ ){

						Block* b = chunk.GetBlock( x, y, z );

						if( !b ){
							continue;
						}

						int wx = chunk.CalcWorldCoordX( x );
						int wz = chunk.CalcWorldCoordZ( z );


						for( intV3& v : vArray ){
							if( y + v.y == -1 || y + v.y == chunk.HEIGHT ){
								continue;
							}

							if( GetBlock( wx + v.x, y + v.y, wz + v.z ) == nullptr ){
								goto BREAK_LABEL;
							}
						}

						b->SetIsDraw( false );
						BREAK_LABEL:;
					}
				}
			}


		}
	}
}

#include "ChunkFiler.h"

void World::Test(){

	if( GetKeyDown( 'U' ) ){
		ChunkFiler filer;
		filer.Write( m_chunkMap[0][0] );
		m_chunkMap[0].erase( 0 );
	}

	if( GetKeyDown( 'I' ) ){
		ChunkFiler filer;
		filer.Read( m_chunkMap[0][0] );
	}
}
