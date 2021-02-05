#include "stdafx.h"
#include "World.h"
#include "ChunkFiler.h"
#include "Player.h"
#include "IntRect.h"
#include "BiomeManager.h"
#include "DropItem.h"
#include "Light.h"
#include "BlockFactory.h"

namespace {
	const float timeBlockDurabilityValueRecover = 0.4f;
	const int randomDrop = Block::WIDTH / 2.5f;	//らんちゅうのはんい。
	std::mt19937 random((std::random_device())());	//らんちゅう。
}

World::World(){
	bool result = infoFile.Read();

	if( result == false ){
		//ファイルが存在しない場合に新しく作成する。
		infoFile.SetRandomSeed();
		infoFile.Write();
	}

	//シード値をそれぞれに設定する。
	BiomeManager::GetInstance().GenerateSeed( infoFile.GetSeedGenerator() );
	m_mapMaker.Init( this, infoFile.GetSeedGenerator() );

	//名前を設定
	SetName(L"World");

	//ブロッククラスにポインタを設定
	Block::SetWorldPtr(this);
}

World::~World(){
	//エンティティたちの削除
	for( Entity* e : m_entities ){
		e->SetWorld( nullptr );
		DeleteGO( e );
	}

	//チャンク削除
	Block::m_sDestroyMode = true;
	m_chunkMap.clear();
	Block::m_sDestroyMode = false;

	//ブロッククラスのポインタを設定
	Block::SetWorldPtr(nullptr);
}

void World::PostUpdate(){

	Player* player = GetPlayer();
	if( player ){
		const CVector3 pPosV = player->GetPos() / Block::WIDTH;

		//チャンク読み込み範囲正方形。
		IntRect chunkSquare = IntRect::CreateWithCenter(
			IntVector3( CalcChunkCoord( (int)pPosV.x ), 0, CalcChunkCoord( (int)pPosV.z ) ),
			{ m_chunkLoadRange, 1, m_chunkLoadRange } );

		//読み込まれていないチャンクを読み込む。
		chunkSquare.for_each( [&]( int x, int, int z ){
			LoadChunk( x, z );
		} );

		//範囲外のチャンクを書き出してメモリから消す。
		for( auto itr = m_chunkMap.begin(); itr != m_chunkMap.end();){

			Chunk& chunk = itr->second;
			const int x = chunk.GetX(), z = chunk.GetZ();

			if( !chunkSquare.IsInclude( x, 0, z ) ){

				ChunkFiler filer;
				filer.Write( chunk );
				itr = m_chunkMap.erase( itr );

			} else{
				itr++;
			}
		}
	}

	{
		//範囲内のブロックのコリジョンを有効化する。範囲外は無効化する。

		//無効化ループ
		for( auto itr = m_activeCollisions.begin(); itr != m_activeCollisions.end(); ){

			const IntVector3& p = *itr;
			bool disable = true;

			//全エンティティをループ
			for( Entity* e : m_entities ){

				if (!e->UseBulletColision()) {
					continue;
				}

				const IntRect eCube = IntRect::CreateWithCenter(
					IntVector3( e->GetPos() / Block::WIDTH ), m_collisionEnableRange );

				if( eCube.IsInclude( p ) ){
					//1体でもエンティティが範囲内なら無効化しない。
					itr++;
					disable = false;
					break;
				}

			}

			if( disable ){

				//誰の範囲内でもなければ無効化。
				Block* b = GetBlock( p.x, p.y, p.z );
				if( b )b->DisableCollision();
				itr = m_activeCollisions.erase( itr );
			}
		}

		//有効化ループ。
		for( Entity* e : m_entities ){

			if (!e->UseBulletColision()) {
				continue;
			}

			const IntRect eCube = IntRect::CreateWithCenter(
				IntVector3( e->GetPos() / Block::WIDTH ), m_collisionEnableRange );

			//各エンティティの近くのブロックをループ
			eCube.for_each( [&]( int x, int y, int z ){

				Block* b = GetBlock( x, y, z );

				if( b && !b->IsCollisionEnabled() ){
					//有効化して有効セットに入れる。
					b->EnableCollision();
					m_activeCollisions.emplace( x, y, z );
				}

			} );
		}
	}
}

Player * World::GetPlayer(){
	if( m_entities.count( m_player ) != 0 ){
		return  m_player;
	}
	return nullptr;
}

Block * World::GetBlock( int x, int y, int z ){
	if( y < 0 || Chunk::HEIGHT <= y ){
		return nullptr;
	}

	Chunk* chunk = GetChunkFromWorldPos( x, z );
	if( chunk ){
		x = Chunk::CalcInChunkCoord( x );
		z = Chunk::CalcInChunkCoord( z );
		return chunk->GetBlock( x, y, z );
	}
	return nullptr;
}

void World::GetBlocks(CVector3 aabbmin, CVector3 aabbmax, std::vector<Block*>& return_blocks) {
	aabbmin -= Block::WIDTH;
	aabbmax += Block::WIDTH;
	CVector3 aabbSize = aabbmax - aabbmin;
	for (float x = 0.0f; x < aabbSize.x - FLT_EPSILON; x = min(aabbSize.x, x + Block::WIDTH)) {
		for (float y = 0.0f; y < aabbSize.y - FLT_EPSILON; y = min(aabbSize.y, y + Block::WIDTH)) {
			for (float z = 0.0f; z < aabbSize.z - FLT_EPSILON; z = min(aabbSize.z, z + Block::WIDTH)) {
				CVector3 pos = aabbmin + CVector3(x,y,z);
				pos /= Block::WIDTH;
				auto block = GetBlock(pos);
				if (block != nullptr) {
					return_blocks.push_back(block);
				}
			}
		}
	}
}

void World::SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );
	chunk->SetBlock( x, y, z, std::move( block ) );
}

char* World::GetLightData(int x, int y, int z) {
	if (y < 0 || Chunk::HEIGHT <= y) {
		return nullptr;
	}

	Chunk* chunk = GetChunkFromWorldPos(x, z);
	if (chunk) {
		x = Chunk::CalcInChunkCoord(x);
		z = Chunk::CalcInChunkCoord(z);
		return chunk->GetLightData(x, y, z);
	}
	return nullptr;
}

char* World::GetSkyLightData(int x, int y, int z) {
	if (y < 0 || Chunk::HEIGHT <= y) {
		return nullptr;
	}

	Chunk* chunk = GetChunkFromWorldPos(x, z);
	if (chunk) {
		x = Chunk::CalcInChunkCoord(x);
		z = Chunk::CalcInChunkCoord(z);
		return chunk->GetSkyLightData(x, y, z);
	}
	return nullptr;
}

Chunk* World::GetChunk( int x, int z ){
	auto pair = std::make_pair( x, z );
	if( m_chunkMap.count( pair ) == 0 ){
		return nullptr;
	}
	return &m_chunkMap.at( pair );
}

Chunk * World::CreateChunk( int x, int z ){
	auto itr = m_chunkMap.find( std::make_pair( x, z ) );
	if( itr != m_chunkMap.end() ){
		return &itr->second;
	}

	Chunk* chunk = &m_chunkMap[std::make_pair( x, z )];

	chunk->SetChunkPos( x, z );

	//ファイルにチャンクがあれば、それを読む。
	ChunkFiler filer;
	filer.Read( *chunk );

	return chunk;
}

void World::LoadChunk( int x, int z ){
	if( !IsExistChunk( x, z ) ){
		Chunk* chunk = CreateChunk( x, z );

		//ファイルにチャンクが存在しなかったか、存在はしたが生成が済んでない場合。
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );
		}

		//埋まったブロックを非表示にする。
		ChunkCulling( *chunk );

		//スカイライトの計算を行う
		SkyLight skylight(this);
		skylight.CalcSkyLight(chunk);
	} else{
		Chunk* chunk = GetChunk( x, z );

		//チャンクが存在はしたが生成が住んでない場合。
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );

			//埋まったブロックを非表示にする。
			ChunkCulling( *chunk );

			//スカイライトの計算を行う
			SkyLight skylight(this);
			skylight.CalcSkyLight(chunk);
		}
	}
}

void World::ChunkCulling( Chunk& chunk ){

	//6方向を収めた配列。
	static constexpr IntVector3 vArray[]{
		{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
	};

	//チャンクの直方体。
	IntRect chunkRect( { 0, 0, 0 }, { chunk.WIDTH , chunk.HEIGHT, chunk.WIDTH } );

	//チャンク内の全ブロックをループ。
	chunkRect.for_each( [&]( int x, int y, int z ){
		Block* b = chunk.GetBlock( x, y, z );

		if( !b ){
			return;
		}

		//チャンク座標からワールド座標へ変換。
		int wx = chunk.CalcWorldCoordX( x );
		int wz = chunk.CalcWorldCoordZ( z );

		bool doCulling = true;

		//上下左右前後を調べるループ。
		for( const IntVector3& v : vArray ){
			if( y + v.y == -1 || y + v.y == chunk.HEIGHT ){
				continue;
			}

			auto neighbor = GetBlock(wx + v.x, y + v.y, wz + v.z);
			if( neighbor == nullptr || BlockFactory::GetIsOpacity(neighbor->GetBlockType()) == false ){//ブロックない or 透明ブロック
				doCulling = false;
				break;
			}
		}

		if( doCulling )b->SetIsDraw( false );
	} );
}

void World::DamegeBlock( const CVector3& pos ){
	//タイマーを0にする
	m_timer = 0.0f;
	int x = (int)std::floorf( pos.x );
	int y = (int)std::floorf( pos.y );
	int z = (int)std::floorf( pos.z );
	if (y == RandomMapMaker::m_minHeight)
	{
		return;
	}

	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	m_block = GetBlock(x, y, z);
	if (m_block == nullptr)
	{
		return;
	}
	//ブロックのHPを減らす、とりあえず1入れてる
	m_block->ReduceHP(1);
	//ブロックのHPが0以上ならこれで終わり
	if (m_block->GetHP() > 0)
	{
		return;
	}
	//ブロックをポップ。
	{
		//ドロップアイテムを作成。
		DropItem* dropItem = DropItem::CreateDropItem( this, GetBlock( x, y, z )->GetBlockType() );
		CVector3 addPos = CVector3::Zero();
		if (random() % 2 > 0) {
			addPos.x += rand() % randomDrop;
		}
		else {
			addPos.x -= rand() % randomDrop;
		}

		if (random() % 2 > 0) {
			addPos.z += rand() % randomDrop;
		}
		else {
			addPos.z += rand() % randomDrop;
		}
		dropItem->SetPos( CVector3( x + 0.5f, y + 0.5f, z + 0.5f ) * Block::WIDTH + addPos);
	}
	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );

	chunk->DeleteBlock(x, y, z);
	AroundBlock(pos);
	//ブロック破壊されたらnullにする
	m_block = nullptr;

}

void World::DestroyBlockNoDrop(const IntVector3& pos) {
	Chunk* chunk = GetChunkFromWorldPos(pos.x, pos.z);
	if (!chunk) {
		return;
	}
	int x = Chunk::CalcInChunkCoord(pos.x);
	int z = Chunk::CalcInChunkCoord(pos.z);

	//破壊
	chunk->DeleteBlock(x, pos.y, z);
	AroundBlock({ (float)pos.x,(float)pos.y,(float)pos.z });
}

bool World::PlaceBlock( const CVector3& pos, std::unique_ptr<Block> block ){
	int x = (int)std::floorf( pos.x );
	int y = (int)std::floorf( pos.y );
	int z = (int)std::floorf( pos.z );
	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );

	if (block->GetBlockType() == enCube_BedHead || block->GetBlockType() == enCube_BedLeg) {
		//ベッド
		CVector3 pos2 = pos;
		if (block->GetBlockType() == enCube_BedHead) {
			pos2 += CVector3(block->GetMukiDir().x, block->GetMukiDir().y, block->GetMukiDir().z)*-1.0f;
		}
		else {
			pos2 += CVector3(block->GetMukiDir().x, block->GetMukiDir().y, block->GetMukiDir().z);
		}
		int x2 = (int)std::floorf(pos2.x);
		int y2 = (int)std::floorf(pos2.y);
		int z2 = (int)std::floorf(pos2.z);
		x2 = Chunk::CalcInChunkCoord(x);
		z2 = Chunk::CalcInChunkCoord(z);

		if (!chunk->CanPlaceBlock(x, y, z) || !chunk->CanPlaceBlock(x2, y2, z2)) {
			return false;
		}

		//ペアのパーツ作成
		std::unique_ptr<Block> block2;
		if (block->GetBlockType() == enCube_BedHead) {
			block2 = BlockFactory::CreateBlock(enCube_BedLeg, block->GetMuki());
		}
		else {
			block2 = BlockFactory::CreateBlock(enCube_BedHead, block->GetMuki());
		}

		chunk->PlaceBlock(x, y, z, std::move(block));
		chunk->PlaceBlock(x2, y2, z2, std::move(block2));

		AroundBlock(pos);
		AroundBlock(pos2);
	}
	else if (block->GetBlockType() == enCube_DoorUp || block->GetBlockType() == enCube_DoorDown) {
		//ドア
		CVector3 pos2 = pos;
		if (block->GetBlockType() == enCube_DoorUp) {
			pos2.y -= 1.0f;
		}
		else {
			pos2.y += 1.0f;
		}
		int x2 = (int)std::floorf(pos2.x);
		int y2 = (int)std::floorf(pos2.y);
		int z2 = (int)std::floorf(pos2.z);
		x2 = Chunk::CalcInChunkCoord(x);
		z2 = Chunk::CalcInChunkCoord(z);

		if (!chunk->CanPlaceBlock(x, y, z) || !chunk->CanPlaceBlock(x2, y2, z2)) {
			return false;
		}

		//ペアのパーツ作成
		std::unique_ptr<Block> block2; 
		if (block->GetBlockType() == enCube_DoorUp) {
			block2 = BlockFactory::CreateBlock(enCube_DoorDown);
		}
		else {
			block2 = BlockFactory::CreateBlock(enCube_DoorUp);
		}

		chunk->PlaceBlock(x, y, z, std::move(block));
		chunk->PlaceBlock(x2, y2, z2, std::move(block2));

		chunk->GetBlock(x2,y2,z2)->SetMuki(chunk->GetBlock(x, y, z)->GetMuki());

		AroundBlock(pos);
		AroundBlock(pos2);
	}
	else {
		//通常のブロックの設置
		if (!chunk->PlaceBlock(x, y, z, std::move(block))) {
			return false;
		}
		AroundBlock(pos);
	}

	return true;
}

void World::AroundBlock( const CVector3& pos ){
	const int posSize = 6;

	CVector3 posList[posSize];
	posList[0] = CVector3( 1.f, 0.f, 0.f );
	posList[1] = CVector3( -1.f, 0.f, 0.f );
	posList[2] = CVector3( 0.f, 1.f, 0.f );
	posList[3] = CVector3( 0.f, -1.f, 0.f );
	posList[4] = CVector3( 0.f, 0.f, 1.f );
	posList[5] = CVector3( 0.f, 0.f, -1.f );

	for( int i = 0; i < posSize; i++ ){
		CVector3 pos2 = CVector3::Zero();
		pos2.x = pos.x + posList[i].x;
		pos2.y = pos.y + posList[i].y;
		pos2.z = pos.z + posList[i].z;

		Block* block = GetBlock( pos2 );
		if( block == nullptr )
			continue;

		bool doNotCulling = false;
		for( int j = 0; j < posSize; j++ ){
			CVector3 pos3 = CVector3::Zero();
			pos3.x = pos2.x + posList[j].x;
			pos3.y = pos2.y + posList[j].y;
			pos3.z = pos2.z + posList[j].z;

			auto neighbor = GetBlock(pos3);
			if (neighbor == nullptr || BlockFactory::GetIsOpacity(neighbor->GetBlockType()) == false) {//ブロックない or 透明ブロック
				doNotCulling = true;
				break;
			}
		}

		block->SetIsDraw( doNotCulling );
	}
}

void World::Update()
{
	if (m_block != nullptr) {
		m_timer += GetDeltaTimeSec();
		if (m_timer >= timeBlockDurabilityValueRecover)
		{
			m_timer = 0.0f;
			m_block->RestoresBlockDurabilityValue();
			m_block = nullptr;
		}

	}
}