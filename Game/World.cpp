#include "stdafx.h"
#include "World.h"
#include "ChunkFiler.h"
#include "Player.h"

void World::PostUpdate(){
	const CVector3 pPosV = m_player->GetPosition() / Block::WIDTH;

	const int pPosX = CalcChunkCoord( (int)pPosV.x );
	const int pPosZ = CalcChunkCoord( (int)pPosV.z );
	const int cl = m_chunkLoadRange;

	//読み込まれていないチャンクを読み込む。
	for( int x = pPosX - cl; x <= pPosX + cl; x++ ){
		for( int z = pPosZ - cl; z <= pPosZ + cl; z++ ){
			if( !IsExistChunk( x, z ) ){
				Chunk* chunk = CreateChunk( x, z );

				//ファイルから読み込む。
				ChunkFiler filer;
				bool readResult = filer.Read( *chunk );

				//ファイルにチャンクが存在しなかったか、存在はしたが生成が済んでない場合。
				if( !chunk->IsGenerated() ){
					m_mapMaker.GenerateChunk( *chunk );
				}

				//埋まったブロックを非表示にする。
				ChunkCulling( *chunk );
			} else{
				Chunk* chunk = GetChunk( x, z );
				//チャンクが存在はしたが生成が住んでない場合。
				if( !chunk->IsGenerated() ){
					m_mapMaker.GenerateChunk( *chunk );
					ChunkCulling( *chunk );
				}
			}
		}
	}

	//範囲外のチャンクを書き出してメモリから消す。
	for( auto& [coord, chunk] : m_chunkMap ){
		const int x = chunk.GetX(), z = chunk.GetZ();
		if( !(pPosX - cl <= x && x <= pPosX + cl && pPosZ - cl <= z && z <= pPosZ + cl) ){
			ChunkFiler filer;
			filer.Write( chunk );
			m_chunkMap.erase( std::make_pair( chunk.GetX(), chunk.GetZ() ) );
		}
	}
	
}

void World::SetPlayer( Player* player, bool recursive ){
	m_player = player;
	if( recursive )
		player->SetWorld( this , false);
}

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
	auto pair = std::make_pair( x, z );
	if( m_chunkMap.count( pair ) == 0 ){
		return nullptr;
	}
	return &m_chunkMap.at(pair);
}

Chunk * World::CreateChunk( int x, int z ){
	auto itr = m_chunkMap.find(std::make_pair( x, z ));
	if( itr != m_chunkMap.end() ){
		return &itr->second;
	}

	Chunk* chunk = &m_chunkMap[std::make_pair( x, z )];

	
	chunk->SetChunkPos( x, z );
	return chunk;
}

void World::AllChunkCulling(){
	//全チャンクをループ
	for( auto& chunkPair : m_chunkMap ){
		ChunkCulling( chunkPair.second );
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
