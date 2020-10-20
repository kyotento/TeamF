#pragma once

template<class Stream>
class ChunkFlags{
public:
	ChunkFlags( Stream& stream , int32_t chunkNo ) : stream( stream ) , chunkNo(chunkNo){
		static_assert( std::is_same<Stream, std::ifstream>::value ||
					   std::is_same<Stream, std::fstream>::value );
	}

	//! フラグを読み込み。
	void ReadFlags(){
		stream.seekg( chunkNo * Flag::Num / 8 );
		stream.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	}

	//! フラグを書き込み。Streamがfstreamのとき専用。
	void WriteFlags(){
		if constexpr( std::is_same<Stream, std::fstream>::value ){
			stream.seekp( chunkNo * Flag::Num / 8 );
			stream.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
		}
	}

	//! チャンクが存在するフラグを立てる。
	void SetExist(){
		flags |= GetFlagMask( Flag::Exist );
	}
	//! チャンクが生成済みのフラグを立てる。
	void SetGenerated(){
		flags |= GetFlagMask( Flag::Generated );
	}
	//! チャンクが存在するフラグを取得。
	bool IsExist(){
		return flags & GetFlagMask( Flag::Exist );
	}
	//! チャンクが生成済みのフラグを取得。
	bool IsGenerated(){
		return flags & GetFlagMask( Flag::Generated );
	}

	enum Flag{
		Exist,
		Generated,
		Num,
	};

private:
	//! 各フラグのチャンク番号に合わせたマスクを取得。
	int8_t GetFlagMask( Flag f){
		//1チャンクごとにNum個のフラグがある。
		//自分のチャンクがさす場所に移動したのち、目的のフラグ " f " に移動する。
		return  1 << ( chunkNo * Flag::Num % 8 + f );
	}

	//! チャンク番号
	int32_t chunkNo;

	//! 読み書き用ファイルストリーム
	Stream& stream;

	//! フラグの格納された数値
	int8_t flags;
};

using ChunkFlagsEnum = ChunkFlags<std::fstream>::Flag;