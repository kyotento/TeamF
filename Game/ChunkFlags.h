#pragma once

template<class Stream>
class ChunkFlags{
public:
	ChunkFlags( Stream& stream ) : stream( stream ){
		static_assert( std::is_same<Stream, std::ifstream>::value ||
					   std::is_same<Stream, std::ofstream>::value||
					   std::is_same<Stream, std::fstream>::value );
	}

	//! フラグを読み込み。
	void ReadFlags(){
		stream.seekg( 0 );
		stream.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	}

	//! フラグを書き込み。Streamがfstreamのとき専用。
	void WriteFlags(){
		if constexpr( std::is_same<Stream, std::ifstream>::value == false ){
			stream.seekp( 0 );
			stream.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
		} else{
			_ASSERTE( false && "std::ifstreamで初期化するとこの関数は使えません。" );
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
	bool IsExist() const {
		return flags & GetFlagMask( Flag::Exist );
	}
	//! チャンクが生成済みのフラグを取得。
	bool IsGenerated() const {
		return flags & GetFlagMask( Flag::Generated );
	}

	enum Flag{
		Exist,
		Generated,
		Num,
	};

private:
	//! 各フラグのチャンク番号に合わせたマスクを取得。
	int8_t GetFlagMask( Flag f) const {
		return  1 << f;
	}

	//! 読み書き用ファイルストリーム
	Stream& stream;

	//! フラグの格納された数値
	int8_t flags = 0;
};

using ChunkFlagsEnum = ChunkFlags<std::fstream>::Flag;