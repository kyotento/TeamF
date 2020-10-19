//! @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"

class Player;

//! @brief Block オブジェクトを保持するワールド
//! @author Takayama
class World : public IGameObject{
public:

	//! @brief 更新関数。チャンクをストレージに退避させる処理をする。
	void PostUpdate() override;

	//! @brief Player をセットする。
	//! @param recursive trueなら Player::SetWorld(this, false) も呼び出す。
	void SetPlayer( Player* player, bool recursive );

	Player* GetPlayer(){
		return m_player;
	}

	//! @brief ワールドを生成する。 
	void Generate(){
		m_mapMaker.Init(this);
		AllChunkCulling();
	}

	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::roundf( pos.x );
		int y = (int)std::roundf( pos.y );
		int z = (int)std::roundf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock( int x, int y, int z );

	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = (int)std::roundf( pos.x );
		int y = (int)std::roundf( pos.y );
		int z = (int)std::roundf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block );

	//=========チャンクの取得に関する関数。==============

	//! @brief チャンクを取得。
	//! @param x,z チャンク座標。
	//! @return チャンクが存在しない場合、nullptr。
	Chunk* GetChunk( int x, int z );

	//! @brief チャンクを生成し、取得。 
	//! @param x,z チャンク座標
	//! @return すでに存在する場合は存在するチャンクを返す。
	Chunk* CreateChunk( int x, int z );

	//! @brief チャンクが存在するか調べる。
	//! @param x,z チャンク座標
	bool IsExistChunk( int x, int z ){
		return m_chunkMap.count( std::make_pair( x, z ) ) != 0;
	}
	

	//==========チャンクの取得に関する関数のワールド座標版。==============

	//! @brief #GetChunk(int,int) のワールド座標版。
	Chunk* GetChunkFromWorldPos( int x, int z ){
		return GetChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//! @brief #CreateChunk(int,int) のワールド座標版。
	Chunk* CreateChunkFromWorldPos( int x, int z ){
		return CreateChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//! @brief #IsExistChunk(int,int) のワールド座標版。
	bool IsExistChunkFromWorldPos( int x, int z ){
		return m_chunkMap.count( std::make_pair( CalcChunkCoord(x), CalcChunkCoord(z) ) ) != 0;
	}



	//! @briefチャンク座標を計算
	//! @param ワールド座標のxかz。
	//! @return チャンク座標のxかz。
	static int CalcChunkCoord( int num ){
		if( num < 0 )num -= Chunk::WIDTH - 1;
		return num / Chunk::WIDTH;
	}

	void Test(const CVector3& pos);
private:
	//! @brief ワールド生成後に埋まっているブロックを非表示にする。
	void AllChunkCulling();
	//! @brief チャンクごとに埋まっているブロックを非表示にする
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapMaker;
	std::map<std::pair<int, int>, Chunk> m_chunkMap;

	Player* m_player = nullptr;

	int m_chunkLoadRange = 2;
};

