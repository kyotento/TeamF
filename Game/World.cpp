#include "stdafx.h"
#include "World.h"

Block * World::GetBlock( int x, int y, int z ){
	Chunk* chunk = GetChunkFromWorldPos( x, z );
	if( chunk ){
		x = Chunk::CalcInChunkCoord( x );
		z = Chunk::CalcInChunkCoord( z );
		return chunk->GetBlock( x, y, z );
	}
	return nullptr;
}

void World::SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if(!chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );
	chunk->SetBlock( x, y, z, std::move( block ) );
}

Chunk* World::GetChunk( int x, int z ){
	try{
		return &m_chunkMap.at( x ).at( z );
	} catch( std::out_of_range ){
		return nullptr;
	}
}

Chunk * World::CreateChunk( int x, int z ){
	Chunk* chunk = &m_chunkMap[x][z];
	chunk->SetChunkPos( x, z );
	return chunk;
}

void World::AllChunkCulling(){
	//全チャンクをループ
	for( auto& chunkMapZ : m_chunkMap ){
		for( auto& chunkPair : chunkMapZ.second ){
			ChunkCulling( chunkPair.second );
		}
	}
}

void World::ChunkCulling( Chunk& chunk ){

	struct intV3{
		int x, y, z;
	};

	static constexpr intV3 vArray[]{
		{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
	};

	//全ブロックのための三重ループ。
	for( int y = 0; y < chunk.HEIGHT; y++ ){
		for( int x = 0; x < chunk.WIDTH; x++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){

				Block* b = chunk.GetBlock( x, y, z );

				if( !b ){
					continue;
				}

				//チャンク座標からワールド座標へ変換。
				int wx = chunk.CalcWorldCoordX( x );
				int wz = chunk.CalcWorldCoordZ( z );

				bool doCulling = true;

				//上下左右前後を調べるループ。
				for( const intV3& v : vArray ){
					if( y + v.y == -1 || y + v.y == chunk.HEIGHT ){
						continue;
					}

					if( GetBlock( wx + v.x, y + v.y, wz + v.z ) == nullptr ){
						doCulling = false;
						break;
					}
				}

				if( doCulling )b->SetIsDraw( false );
			}
		}
	}
}

#include "ChunkFiler.h"

void World::Test( const CVector3& pos ){

	if( GetKeyDown( 'U' ) ){
		Chunk* chunk = GetChunkFromWorldPos( pos.x, pos.z );
		ChunkFiler filer;
		filer.Write( *chunk );
		m_chunkMap[chunk->GetX()].erase( chunk->GetZ() );
	}

	if( GetKeyDown( 'I' ) ){
		Chunk* chunk = CreateChunkFromWorldPos( pos.x, pos.z );
		ChunkFiler filer;
		if( filer.Read( *chunk ) ){
			ChunkCulling( *chunk );
		}
	}
}
