//! @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"
#include "WorldInfoFile.h"
#include "Sun.h"

class Entity;
class Player;
class Boss;
class DropItem;

//! @brief Block オブジェクトを保持するワールド
//! @author Takayama
class World : public IGameObject{
public:
	World();
	~World();

	//! @brief 更新関数。チャンクをストレージに退避させる処理をする。
	void PostUpdate() override;

	//エラーメッセージを表示する。
	void PostRender() override
	{
		DisplayErrorMessage();
	}

	//太陽を取得
	Sun* GetSun() {
		return m_sun.get();
	}

	//! @brief Player をセットする。
	void SetPlayer( Player* player ){
		m_player = player;
	}

	Player* GetPlayer();

	//! @brief Boss をセットする。
	void SetBoss(Boss* boss) {
		m_bosses.insert(boss);
	}
	void RemoveBoss(Boss* boss) {
		m_bosses.erase(boss);
	}
	std::unordered_set<Boss*>& GetBosses() {
		return m_bosses;
	}

	//! @brief Entity をワールドに生成する。引数は対象のコンストラクタ。
	template<class T, class... TArgs>
	T*  CreateEntity( TArgs... ctorArgs ){
		T* newEntity = NewGO<T>( ctorArgs... );
		newEntity->SetWorld( this );
		m_entities.insert( newEntity );
		return newEntity;
	}

	//! @brief Entity をワールドから取り除く。DeleteGOはしない。
	void RemoveEntity( Entity* entity ){
		m_entities.erase( entity );
	}

	//! @brief チャンクを読み込む距離を取得。
	//! @details この値の2倍を辺とする正方形が読み込み範囲。
	int GetChunkLoadRange(){
		return m_chunkLoadRange;
	}
	
	void SetChunkCoadRange(int chunkRange) {
		m_chunkLoadRange = chunkRange;
	}

	//! @brief 指定位置のブロックを取得。
	//! @details 座標はブロック単位
	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock(const IntVector3& pos) {
		return GetBlock(pos.x, pos.y, pos.z);
	}
	Block* GetBlock( int x, int y, int z );

	/// <summary>
	/// 指定のAABB付近のブロックを取得
	/// </summary>
	/// <param name="aabbmin">AABBの小さい方のワールド座標</param>
	/// <param name="aabbmax">AABBの大きい方のワールド座標</param>
	/// <param name="return_blocks">このベクターに範囲内のブロックが追加される</param>
	void GetBlocks(CVector3 aabbmin, CVector3 aabbmax, std::vector<Block*>& return_blocks);

	/// <summary>
	/// ブロックとレイ判定
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="return_hitPos"></param>
	/// <returns></returns>
	Block* RayTestBlock(const CVector3& start, const CVector3& end, CVector3* return_hitPos = nullptr, CVector3* return_hitNormal = nullptr);

	//! @brief 指定位置にブロックをセット。
	//! @details 座標はブロック単位
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
	//ブロックにダメージを与える、プレイヤー用。
	const Block* DamegeBlock(const CVector3& pos, EnTool toolType = EnTool::enTool_None, int toolLevel = 1);

	//無条件にブロックを破壊。
	//座標はワールド座標をBlock::WIDTHで割ったもの
	void DestroyBlock(const IntVector3& pos);

	//無条件にブロックを破壊。アイテムドロップなし
	//座標はワールド座標をBlock::WIDTHで割ったもの
	void DestroyBlockNoDrop(const IntVector3& pos);

	//一つのブロックの周りのブロックのカリング処理をする。
	void AroundBlock(const CVector3& pos);

	//! @brief 指定位置のライト情報を取得。
	//! @details 座標はブロック単位
	char* GetLightData(const IntVector3& pos) {
		return GetLightData(pos.x, pos.y, pos.z);
	}
	char* GetLightData(int x, int y, int z);

	//! @brief 指定位置のスカイライト情報を取得。
	//! @details 座標はブロック単位
	char* GetSkyLightData(const IntVector3& pos) {
		return GetSkyLightData(pos.x, pos.y, pos.z);
	}
	char* GetSkyLightData(int x, int y, int z);

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

	//チャンクデータを保存する。
	void SaveChunk();
	void DisplayErrorMessage();
private:
	//! @brief チャンクをロード。ロード済みなら何もしない。
	bool LoadChunk(int x, int z);

	//! @brief チャンクごとに埋まっているブロックを非表示にする
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapMaker;
	std::map<std::pair<int, int>, Chunk> m_chunkMap;

	std::unique_ptr<Sun> m_sun;//太陽

	Player* m_player = nullptr;
	std::unordered_set<Boss*> m_bosses;

	//! 読み込み範囲を指定。チャンク単位。
	int m_chunkLoadRange = 1;

	//! プレイヤーを中心とした、コリジョンが有効化される範囲。ブロック単位。
	//int m_collisionEnableRange = 2;
	//std::unordered_set<IntVector3> m_activeCollisions;

	//! エンティティ(ブロック以外の動く物)を入れておく配列。
	std::unordered_set<Entity*> m_entities;

	//!シード値などの情報を保存。
	WorldInfoFile infoFile;


	Block* m_block = nullptr;
	float m_timer = 0.0f;
	int m_blockPosX;
	int m_blockPosY;
	int m_blockPosZ;
	CFont m_font;
	bool m_isError = false;
	float m_errorTimer = -1.0f;
}; 

