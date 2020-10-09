#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"


size_t GetSeekSize( const Chunk & chunk ){
	constexpr uint32_t chunkSize = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );
	const int chunkNo = chunk.GetX() * chunk.GetZ();

	return chunkSize * chunkNo;
}

void ChunkFiler::Read( Chunk & chunk ){
	std::ifstream ifs( m_filePath );

	if( !ifs )abort();

	ifs.seekg( GetSeekSize( chunk ) );

	auto& data = chunk.Data();

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){
				int16_t bt;

				ifs.read( reinterpret_cast<char*>( &bt ), 2 );

				if( bt == -1 ){
					continue;
				}

				chunk.SetBlock( x, y, z, BlockFactory::CreateBlock( static_cast<EnCube>( bt ) ) );
			}
		}
	}
}

void ChunkFiler::Write( const Chunk & chunk ){
	std::filesystem::create_directories( m_filePath.parent_path() );

	std::ofstream ofs( m_filePath );

	if( !ofs )abort();

	ofs.seekp( GetSeekSize( chunk ) );

	const auto& data = chunk.Data();

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){

				auto& block = data[x][y][z];

				if( !block ){
					int16_t air = -1;
					ofs.write( reinterpret_cast<char*>( &air ), 2 );
					continue;
				}

				int16_t bt = static_cast<uint16_t>( block->GetBlockType() );

				ofs.write( reinterpret_cast<char*>( &bt ), 2 );

			}
		}
	}
}
