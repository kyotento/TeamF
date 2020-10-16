//! @file
#pragma once

class Chunk;

//! @brief ファイルに対して Chunk オブジェクトの読み書きを行う。
//! @author Takayama
class ChunkFiler{
public:

	//! @param[out] chunk このチャンクにファイルから読み込む。
	//! @retval true ファイルを正常に読み込んだ。
	//! @retval false チャンク情報が存在しなかったなどの理由で読み込みに失敗した。
	bool Read( Chunk& chunk );

	//! @param[in] chunk このチャンクをファイルに書き出す。
	void Write( const Chunk& chunk );
};