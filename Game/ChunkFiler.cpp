#include "stdafx.h"
#include "ChunkFiler.h"
#include "Chunk.h"
#include "BlockFactory.h"

namespace{

#include "ChunkFlags.h"

	//! REGION_W * REGION_W 個のチャンクで構成される正方形を1ファイルとする。
	constexpr int32_t REGION_W = 32;

	//! チャンクが存在するかのフラグと、チャンクが生成済みかのフラグのデータサイズ。
	constexpr int32_t FLAG_BYTES = (ChunkFlagsEnum::Num + 7 ) / 8;

	//! チャンクのデータサイズ(byte)
	constexpr int32_t CHUNK_SIZE = Chunk::WIDTH * Chunk::WIDTH * Chunk::HEIGHT * sizeof( short );

	//! 10進数の文字数を返す。マイナス符号も1とみなす。
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

	//! チャンクを保存するべきchunkファイルを決定する。chunk座標で"chunkX,Z.chk"のファイル名となる。
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

	//チャンクが存在するかどうかのフラグを管理するオブジェクト。
	ChunkFlags flags( ifs );

	//自分のフラグがある場所のバイトを読み込む。
	flags.ReadFlags();

	//チャンクが存在しない。
	if( !flags.IsExist() )return false;

	//チャンクを生成済みにする。
	if( flags.IsGenerated() )chunk.SetGenerated();

	//チャンクデータまでシーク。
	ifs.seekg( FLAG_BYTES );

	for( int x = 0; x < chunk.WIDTH; x++ ){
		for( int y = 0; y < chunk.HEIGHT; y++ ){
			for( int z = 0; z < chunk.WIDTH; z++ ){

				//ブロックのID
				int16_t bt;
				ifs.read( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

				if( bt == EnCube::enCube_None ){
					continue;
				}

				//ブロックの向き
				uint8_t muki;
				ifs.read( reinterpret_cast<char*>( &muki ), sizeof( muki ) );

				//ブロック作成。
				auto block = BlockFactory::CreateBlock( static_cast<EnCube>( bt ), static_cast<Block::enMuki>( muki ) );

				//ブロックの追加情報。
				block->ReadExData( ifs );

				chunk.SetBlock( x, y, z, std::move(block) );
			}
		}
	}

	return true;
}

void ChunkFiler::Write( const Chunk & chunk ){
	std::filesystem::path filePath = GetFilePath( chunk );
	//ディレクトリが無い場合に作成する。
	std::filesystem::create_directories( filePath.parent_path() );
	//元の内容を保持しつつ編集するためにios::inも指定する。
	std::ofstream fs( filePath, std::ios::binary );

	//チャンクが存在するかどうかのフラグを管理するオブジェクト。
	ChunkFlags flags( fs );

	//チャンクが存在するフラグを立てる。
	flags.SetExist();

	//チャンクが生成済みのフラグを立てる。
	if( chunk.IsGenerated() ) flags.SetGenerated();

	//フラグを書き込む。
	flags.WriteFlags();

	//チャンクデータまでシーク。
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

				//ブロックのID
				int16_t bt = static_cast<int16_t>( block->GetBlockType() );
				fs.write( reinterpret_cast<char*>( &bt ), sizeof( bt ) );

				//ブロックの向き
				uint8_t muki = static_cast<uint8_t>( block->GetMuki() );
				fs.write( reinterpret_cast<char*>( &muki ), sizeof( muki ) );

				//ブロックの追加情報
				block->WriteExData( fs );

			}
		}
	}
}