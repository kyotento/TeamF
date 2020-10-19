#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"

namespace{

	//! REGION_W * REGION_W �̃`�����N�ō\������鐳���`��1�t�@�C���Ƃ���B
	constexpr int32_t REGION_W = 32;

	//! �`�����N�����݂��邩�̃t���O�̃f�[�^�T�C�Y�B
	constexpr int32_t FLAG_BYTES = ( REGION_W * REGION_W + 7 ) / 8;

	//! �`�����N�̃f�[�^�T�C�Y(byte)
	constexpr int32_t CHUNK_SIZE = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );

	//! �`�����N�ɗ^����ԍ��B�t�@�C�����̏������݈ʒu�����肷��B0�`(REGION_W * REGION_W - 1)�܂ŁB
	int32_t GetChunkNo( const Chunk & chunk ){
		int x = chunk.GetX() % REGION_W;
		int z = chunk.GetX() % REGION_W;

		if( x < 0 )x += REGION_W;
		if( z < 0 )z += REGION_W;

		return x * REGION_W + z;
	}

	//! 10�i���̕�������Ԃ��B�}�C�i�X������1�Ƃ݂Ȃ��B
	int32_t GetCharLen( int num ){
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

	//! �`�����N��ۑ�����ׂ�region�t�@�C�������肷��Bregion���W��"regionX,Z.regi"�̃t�@�C�����ƂȂ�B
	std::filesystem::path GetFilePath( const Chunk & chunk ){
		int x = chunk.GetX();
		int z = chunk.GetZ();

		//REGION_W������5�Ȃ�A0�`4�̂Ƃ��ƁA-1�`-4�̂Ƃ��ŁA���Ƀ��M�I�����W��0�ɂȂ��Ă��܂��̂ŁA
		//�l���}�C�i�X�̂Ƃ���(REGION_W - 1)�������B��̗Ⴞ��-1��-5�ɂȂ背�M�I�����W��-1�ɂȂ�B
		if( x < 0 )x -= REGION_W - 1;
		if( z < 0 )z -= REGION_W - 1;

		x /= REGION_W;
		z /= REGION_W;

		size_t pathSize = sizeof( "./Save/World/region,.regi" ) + GetCharLen( x ) + GetCharLen( z );

		auto filePath = std::make_unique<char[]>( pathSize );
		sprintf_s( filePath.get(), pathSize, "./Save/World/region%d,%d.regi", x, z );

		return std::filesystem::path( filePath.get() );
	}

}

bool ChunkFiler::Read( Chunk & chunk ){
	const uint32_t chunkNo = GetChunkNo( chunk );
	std::ifstream ifs( GetFilePath( chunk ), std::ios::binary );

	if( !ifs )return false;

	//�����̃t���O������ꏊ�̃o�C�g��ǂݍ��ށB
	uint8_t flags = 0;
	ifs.seekg( chunkNo / 8 );
	ifs.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );

	if( !( ( 1 << chunkNo % 8 )  & flags ) ){
		OutputDebugStringA( "���������B\n" );
		return false;
	}

	//�`�����N�f�[�^�܂ŃV�[�N�B
	ifs.seekg( chunkNo * CHUNK_SIZE + FLAG_BYTES );

	auto& data = chunk.Data();

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){
				uint16_t bt;

				ifs.read( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

				if( bt == 0xffff ){
					continue;
				}

				chunk.SetBlock( x, y, z, BlockFactory::CreateBlock( static_cast<EnCube>( bt ) ) );
			}
		}
	}

	return true;
}

void ChunkFiler::Write( const Chunk & chunk ){
	const uint32_t chunkNo = GetChunkNo( chunk );

	std::filesystem::path filePath = GetFilePath( chunk );
	//�f�B���N�g���������ꍇ�ɍ쐬����B
	std::filesystem::create_directories( filePath.parent_path() );
	//���̓��e��ێ����ҏW���邽�߂�ios::in���w�肷��B
	std::fstream fs( filePath, std::ios::binary | std::ios::out | std::ios::in );

	//�`�����N�����݂��邩�ǂ����̃t���O�B
	uint8_t flags = 0;

	if( fs ){
		//�����̃t���O������ꏊ�̃o�C�g��ǂݍ��ށB
		fs.seekg( chunkNo / 8 );
		fs.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	} else{
		//ios::in�̓t�@�C�������݂��Ȃ��ꍇ�Ɏ��s���邽�߁A���̏ꍇ�͊J�������B
		fs.open( filePath, std::ios::binary | std::ios::out );
		if( !fs )abort();
	}

	//�`�����N�����݂���t���O�𗧂Ă�B
	flags |= ( 1 << chunkNo % 8 );
	fs.seekp( chunkNo / 8 );
	fs.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );


	//�`�����N�f�[�^�܂ŃV�[�N�B
	fs.seekp( chunkNo * CHUNK_SIZE + FLAG_BYTES );

	const auto& data = chunk.Data();

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){

				auto& block = data[x][y][z];

				if( !block ){
					uint16_t air = 0xffff;
					fs.write( reinterpret_cast<char*>( &air ), sizeof( air ) );
					continue;
				}

				uint16_t bt = static_cast<uint16_t>( block->GetBlockType() );

				fs.write( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

			}
		}
	}
}