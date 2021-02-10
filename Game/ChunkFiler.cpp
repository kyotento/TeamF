#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"

namespace{

#include "ChunkFlags.h"

	//! REGION_W * REGION_W �̃`�����N�ō\������鐳���`��1�t�@�C���Ƃ���B
	constexpr int32_t REGION_W = 32;

	//! �`�����N�����݂��邩�̃t���O�ƁA�`�����N�������ς݂��̃t���O�̃f�[�^�T�C�Y�B
	constexpr int32_t FLAG_BYTES = (ChunkFlagsEnum::Num + 7 ) / 8;

	//! �`�����N�̃f�[�^�T�C�Y(byte)
	constexpr int32_t CHUNK_SIZE = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );

	//! 10�i���̕�������Ԃ��B�}�C�i�X������1�Ƃ݂Ȃ��B
	int32_t GetCharLen( int num ){
		if( num == 0 ){
			return 1;
		}

		int32_t minus = 0;

		if( num < 0 ){
			minus = 1;
			num *= -1;
		}

		return int32_t(log10( num )) + 1 + minus;
	}

	//! �`�����N��ۑ�����ׂ�region�t�@�C�������肷��Bregion���W��"regionX,Z.regi"�̃t�@�C�����ƂȂ�B
	std::filesystem::path GetFilePath( const Chunk & chunk ){
		int x = chunk.GetX();
		int z = chunk.GetZ();

		size_t pathSize = sizeof( "./Save/World/Chunk/chunk,.chk" ) + GetCharLen( x ) + GetCharLen( z );

		auto filePath = std::make_unique<char[]>( pathSize );
		sprintf_s( filePath.get(), pathSize, "./Save/World/Chunk/chunk%d,%d.chk", x, z );

		return std::filesystem::path( filePath.get() );
	}

}

bool ChunkFiler::Read( Chunk & chunk ){
	auto path = GetFilePath( chunk );
	std::ifstream ifs( path, std::ios::binary );

	if( !ifs )return false;

	//�`�����N�����݂��邩�ǂ����̃t���O���Ǘ�����I�u�W�F�N�g�B
	ChunkFlags flags( ifs );

	//�����̃t���O������ꏊ�̃o�C�g��ǂݍ��ށB
	flags.ReadFlags();

	//�`�����N�����݂��Ȃ��B
	if( !flags.IsExist() )return false;

	//�`�����N�𐶐��ς݂ɂ���B
	if( flags.IsGenerated() )chunk.SetGenerated();

	//�`�����N�f�[�^�܂ŃV�[�N�B
	ifs.seekg( FLAG_BYTES );

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){
				int16_t bt;

				ifs.read( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

				if( bt == EnCube::enCube_None ){
					continue;
				}

				chunk.SetBlock( x, y, z, BlockFactory::CreateBlock( static_cast<EnCube>( bt ) ) );
			}
		}
	}

	return true;
}

void ChunkFiler::Write( const Chunk & chunk ){
	std::filesystem::path filePath = GetFilePath( chunk );
	//�f�B���N�g���������ꍇ�ɍ쐬����B
	std::filesystem::create_directories( filePath.parent_path() );
	//���̓��e��ێ����ҏW���邽�߂�ios::in���w�肷��B
	std::ofstream fs( filePath, std::ios::binary );

	//�`�����N�����݂��邩�ǂ����̃t���O���Ǘ�����I�u�W�F�N�g�B
	ChunkFlags flags( fs );

	//�`�����N�����݂���t���O�𗧂Ă�B
	flags.SetExist();

	//�`�����N�������ς݂̃t���O�𗧂Ă�B
	if( chunk.IsGenerated() ) flags.SetGenerated();

	//�t���O���������ށB
	flags.WriteFlags();

	//�`�����N�f�[�^�܂ŃV�[�N�B
	fs.seekp( FLAG_BYTES );

	const auto& data = chunk.Data();

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){

				auto& block = data[x][y][z];

				if( !block ){
					int16_t air = EnCube::enCube_None;
					fs.write( reinterpret_cast<char*>( &air ), sizeof( air ) );
					continue;
				}

				int16_t bt = static_cast<uint16_t>( block->GetBlockType() );

				fs.write( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

			}
		}
	}
}