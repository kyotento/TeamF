#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"

namespace{

size_t GetSeekSize( const Chunk & chunk ){
	constexpr uint32_t chunkSize = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );
	const int chunkNo = chunk.GetX() * 32 + chunk.GetZ();

	return chunkSize * chunkNo;
}

int GetCharLen( int num ){
	if( num == 0 ){
		return 1;
	}

	int minus = 0;

	if( num < 0 ){
		minus = 1;
		num *= -1;
	}

	return log10( num ) + 1 + minus;
}

std::filesystem::path GetFilePath( const Chunk & chunk ){
	int x = chunk.GetX() / 32;
	int z = chunk.GetZ() / 32;

	size_t pathSize = sizeof( "./Save/World/region,.regi" ) + GetCharLen( x ) + GetCharLen( z );

	auto filePath = std::make_unique<char[]>( pathSize );
	sprintf_s( filePath.get(), pathSize, "./Save/World/region%d,%d.regi", x, z );

	return std::filesystem::path( filePath.get() );
}

}

void ChunkFiler::Read( Chunk & chunk ){
	std::ifstream ifs( GetFilePath(chunk), std::ios::binary );

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
	std::filesystem::path filePath = GetFilePath( chunk );
	std::filesystem::create_directories( filePath.parent_path() );
	std::ofstream ofs( filePath, std::ios::binary | std::ios::in);

	if( !ofs ){
		ofs.open( filePath, std::ios::binary);
	}

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