#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"

namespace{

	//! REGION_W * REGION_W 個のチャンクで構成される正方形を1ファイルとする。
	constexpr int32_t REGION_W = 32;

	//! チャンクが存在するかのフラグのデータサイズ。
	constexpr int32_t FLAG_BYTES = ( REGION_W * REGION_W + 7 ) / 8;

	//! チャンクのデータサイズ(byte)
	constexpr int32_t CHUNK_SIZE = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );

	//! チャンクに与える番号。ファイル内の書き込み位置を決定する。0～(REGION_W * REGION_W - 1)まで。
	int32_t GetChunkNo( const Chunk & chunk ){
		int x = chunk.GetX() % REGION_W;
		int z = chunk.GetX() % REGION_W;

		if( x < 0 )x += REGION_W;
		if( z < 0 )z += REGION_W;

		return x * REGION_W + z;
	}

	//! 10進数の文字数を返す。マイナス符号も1とみなす。
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

	//! チャンクを保存するべきregionファイルを決定する。region座標で"regionX,Z.regi"のファイル名となる。
	std::filesystem::path GetFilePath( const Chunk & chunk ){
		int x = chunk.GetX();
		int z = chunk.GetZ();

		//REGION_Wがもし5なら、0～4のときと、-1～-4のときで、共にレギオン座標が0になってしまうので、
		//値がマイナスのときは(REGION_W - 1)を引く。上の例だと-1は-5になりレギオン座標は-1になる。
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

	//自分のフラグがある場所のバイトを読み込む。
	uint8_t flags = 0;
	ifs.seekg( chunkNo / 8 );
	ifs.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );

	if( !( ( 1 << chunkNo % 8 )  & flags ) ){
		OutputDebugStringA( "無かった。\n" );
		return false;
	}

	//チャンクデータまでシーク。
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
	//ディレクトリが無い場合に作成する。
	std::filesystem::create_directories( filePath.parent_path() );
	//元の内容を保持しつつ編集するためにios::inも指定する。
	std::fstream fs( filePath, std::ios::binary | std::ios::out | std::ios::in );

	//チャンクが存在するかどうかのフラグ。
	uint8_t flags = 0;

	if( fs ){
		//自分のフラグがある場所のバイトを読み込む。
		fs.seekg( chunkNo / 8 );
		fs.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	} else{
		//ios::inはファイルが存在しない場合に失敗するため、その場合は開き直す。
		fs.open( filePath, std::ios::binary | std::ios::out );
		if( !fs )abort();
	}

	//チャンクが存在するフラグを立てる。
	flags |= ( 1 << chunkNo % 8 );
	fs.seekp( chunkNo / 8 );
	fs.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );


	//チャンクデータまでシーク。
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