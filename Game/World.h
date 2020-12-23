//! @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"
#include "IntVector3.h"
#include "WorldInfoFile.h"

class Entity;
class Player;
class DropItem;

//! @brief Block オブジェクトを保持するワールド
//! @author Takayama
class World : public IGameObject{
public:
	World();

	//! @brief 更新関数。チャンクをストレージに退避させる処理をする。
	void PostUpdate() override;

	//! @brief Player をセットする。
	//! @param recursive trueなら Player::SetWorld(this, false) も呼び出す。
	void SetPlayer( Player* player, bool recursive = false );

	Player* GetPlayer();

	//! @brief Entity をワールドに追加する。
	void AddEntity( Entity* entity ){
		m_entities.insert( entity );
	}

	//! @brief Entity をワールドから取り除く。
	void RemoveEntity( Entity* entity ){
		m_entities.erase( entity );
	}

	//! @brief チャンクを読み込む距離を取得。
	//! @details この値の2倍を辺とする正方形が読み込み範囲。
	int GetChunkLoadRange(){
		return m_chunkLoadRange;
	}

	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock( int x, int y, int z );

	/// <summary>
	/// 指定のAABB範囲内のブロックを取得
	/// </summary>
	/// <param name="aabbmin">AABBの小さい方の座標</param>
	/// <param name="aabbmax">AABBの大きい方の座標</param>
	/// <param name="return_blocks">このベクターに範囲内のブロックが追加される</param>
	void GetBlocks(CVector3 aabbmin, CVector3 aabbmax, std::vector<Block*>& return_blocks);

	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block );

	//ワールド座標をBlock::WIDTHで割ったものを元に。
	//ブロックを設置、プレイヤー用。
	//設置に成功したらtrue。
	bool PlaceBlock(const CVector3& pos, std::unique_ptr<Block> block);

	//ワールド座標をBlock::WIDTHで割ったものを元に。
	//ブロックを破壊、プレイヤー用。
	void DeleteBlock(const CVector3& pos);

	//一つのブロックの周りのブロックのカリング処理をする。
	void AroundBlock(const CVector3& pos);
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
	//! @briefエンティティを取得
	//! @return エンティティの配列。
	std::unordered_set<Entity*>& GetEntities()
	{
		return m_entities;
	}
private:
	//! @brief チャンクをロード。ロード済みなら何もしない。
	void LoadChunk(int x, int z);

	//! @brief チャンクごとに埋まっているブロックを非表示にする
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapMaker;
	std::map<std::pair<int, int>, Chunk> m_chunkMap;

	Player* m_player = nullptr;

	//! 読み込み範囲を指定。チャンク単位。
	int m_chunkLoadRange = 1;

	//! プレイヤーを中心とした、コリジョンが有効化される範囲。ブロック単位。
	int m_collisionEnableRange = 2;
	std::unordered_set<IntVector3> m_activeCollisions;

	//! エンティティ(ブロック以外の動く物)を入れておく配列。
	std::unordered_set<Entity*> m_entities;

	//!シード値などの情報を保存。
	WorldInfoFile infoFile;
};

