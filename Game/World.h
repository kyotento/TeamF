/// @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"

/// @brief Block オブジェクトを保持するワールド
/// @author Takayama
class World{
public:

	/// @brief ワールドを生成する。 
	void Generate(){
		m_mapmMaker.Init(this);
		AllChunkCulling();
	}

	Block* GetBlock( const CVector3& pos ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock( int x, int y, int z );

	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = std::roundf( pos.x );
		int y = std::roundf( pos.y );
		int z = std::roundf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block );

	/// @brief チャンクを取得。
	/// @param x,z チャンク座標
	/// @throw std::out_of_range 
	Chunk* GetChunk( int x, int z );

	/// @brief チャンクを生成し、取得。 
	/// @details すでに存在する場合は存在するチャンクを返す。
	/// @param x,z チャンク座標
	Chunk* CreateChunk( int x, int z );
	
	//@brief #GetChunk(int,int) のワールド座標版。
	Chunk* GetChunkFromWorldPos( int x, int z ){
		return GetChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//@brief #CreateChunk(int,int) のワールド座標版。
	Chunk* CreateChunkFromWorldPos( int x, int z ){
		return CreateChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	/// @briefチャンク座標を計算
	static int CalcChunkCoord( int num ){
		if( num < 0 )num -= Chunk::WIDTH - 1;
		return num / Chunk::WIDTH;
	}

	void Test(const CVector3& pos);
private:
	/// @brief ワールド生成後に埋まっているブロックを非表示にする。
	void AllChunkCulling();
	/// @brief チャンクごとに埋まっているブロックを非表示にする
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapmMaker;
	std::map<int, std::map<int, Chunk>> m_chunkMap;
};

