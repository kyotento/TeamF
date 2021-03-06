#include "stdafx.h"
#include "World.h"
#include "ChunkFiler.h"
#include "Player.h"
#include "IntRect.h"
#include "BiomeManager.h"
#include "DropItem.h"
#include "Light.h"
#include "BlockFactory.h"
#include "Boss.h"

namespace {
	constexpr int ENTITY_DELETE_CHUNK_AREA_OFFSET = 2;//表示チャンク+ENTITY_DELETE_CHUNK_AREA_OFFSETの範囲を超えた場所のエンティティは死ぬ
	const float timeBlockDurabilityValueRecover = 0.4f;
	const int randomDrop = Block::WIDTH / 2.5f;	//らんちゅうのはんい。
	std::mt19937 random((std::random_device())());	//らんちゅう。
	const int maxHeight = 63;	//ブロックの設置できる最大の高さ。
	const wchar_t* errorMessage = L"これ以上の高さにブロックは置けません。";
	const int errorTime = 1.0f;	//エラーメッセージを表示する時間。
	std::vector<Block*> rayColblocks;
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

	//太陽
	m_sun = std::make_unique<Sun>();
}

World::~World(){
	//エンティティたちの削除
	for( Entity* e : m_entities ){
		e->SetWorld( nullptr );
		InstantDeleteGO( e );
	}
	//チャンクデータを保存する。
	SaveChunk();

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
		bool isChunkReload = false;//チャンクの作成or削除したか?

		//チャンク読み込み範囲正方形。
		IntRect chunkSquare = IntRect::CreateWithCenter(
			IntVector3( CalcChunkCoord( (int)pPosV.x ), 0, CalcChunkCoord( (int)pPosV.z ) ),
			{ m_chunkLoadRange, 1, m_chunkLoadRange } );

		//読み込まれていないチャンクを読み込む。
		chunkSquare.for_each( [&]( int x, int, int z ){
			if (LoadChunk(x, z)) {
				isChunkReload = true;
			}
		} );

		//範囲外のチャンクを書き出してメモリから消す。
		for( auto itr = m_chunkMap.begin(); itr != m_chunkMap.end();){

			Chunk& chunk = itr->second;
			const int x = chunk.GetX(), z = chunk.GetZ();

			if( !chunkSquare.IsInclude( x, 0, z ) ){

				ChunkFiler filer;
				filer.Write( chunk );
				itr = m_chunkMap.erase( itr );
				isChunkReload = true;

			} else{
				itr++;
			}
		}

		//チャンク読み込み範囲内のエンティティのみ有効化
		if (isChunkReload) {
			CVector3 min, max;
			min.x = chunkSquare.minPos.x; min.y = chunkSquare.minPos.y; min.z = chunkSquare.minPos.z;
			min *= Chunk::WIDTH * Block::WIDTH;
			max.x = chunkSquare.minPos.x + chunkSquare.edgeLength.x; max.y = chunkSquare.minPos.y + chunkSquare.edgeLength.y; max.z = chunkSquare.minPos.z + chunkSquare.edgeLength.z;
			max *= Chunk::WIDTH * Block::WIDTH;

			CVector3 minBig = min, maxBig = max;
			minBig -= Chunk::WIDTH * Block::WIDTH * ENTITY_DELETE_CHUNK_AREA_OFFSET;
			maxBig += Chunk::WIDTH * Block::WIDTH * ENTITY_DELETE_CHUNK_AREA_OFFSET;

			min.y = minBig.y = 0.0f;
			max.y = maxBig.y = (Chunk::HEIGHT+10) * Block::WIDTH;

			for (Entity* e : m_entities) {
				if (e == player) { continue; }//プレイヤは無視
				if (CMath::ColAABBs(min, max, e->GetPos() - CVector3::One(), e->GetPos() + CVector3::One())) {
					e->SetEnableEntity(true);
				}
				else {
					if (!CMath::ColAABBs(minBig, maxBig, e->GetPos() - CVector3::One(), e->GetPos() + CVector3::One())) {
						//離れまくったエンティティは削除
						DeleteGO(e);
					}
					else {
						e->SetEnableEntity(false);
					}
				}
			}
		}
	}

	{
		if (m_block != nullptr) {
			if (GetBlock(m_blockPosX, m_blockPosY, m_blockPosZ) == nullptr)
			{
				m_block = nullptr;
			}
			else {
				m_timer += GetDeltaTimeSec();
				if (m_timer >= timeBlockDurabilityValueRecover)
				{
					m_timer = 0.0f;
					m_block->RestoresBlockDurabilityValue();
					m_block = nullptr;
				}
			}
		}
		//範囲内のブロックのコリジョンを有効化する。範囲外は無効化する。
		/*
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
		*/
	}	

	if (GetKeyInput(VK_CONTROL) && GetKeyDown(VK_F2)) {
		//ボス作成
		Boss* boss = CreateEntity<Boss>();
		boss->SetPos(m_player->GetPos() + CVector3::AxisZ() * Block::WIDTH*5.0f);
	}
}

void World::DisplayErrorMessage()
{
	if (m_errorTimer >= 0.0f)
	{
		m_font.DrawScreenPos(errorMessage, { 50.f,505.f }, CVector4::Red(), { 0.8f,0.8f },
			CVector2::Zero(),
			0.0f,
			DirectX::SpriteEffects_None,
			0.4f
		);
		m_errorTimer -= GetDeltaTimeSec();
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

Block* World::RayTestBlock(const CVector3& start, const CVector3& end, CVector3* return_hitPos, CVector3* return_hitNormal) {
	constexpr float oneLength =  Block::WIDTH;
	float reyLength = (start - end).Length();
	CVector3 sampPos = start;
	CVector3 dir = end - start; dir.Normalize();
	for (float length = 0.0f; length <= reyLength + FLT_EPSILON; length += oneLength) {		
		sampPos += dir * oneLength;

		rayColblocks.clear();
		GetBlocks(sampPos - CVector3(Block::WIDTH), sampPos + CVector3(Block::WIDTH), rayColblocks);

		float distance = -1.0f;
		std::pair<CVector3, Block*> hitPair = std::make_pair(CVector3::Zero(), nullptr);
		CVector3 hitAABBcenter;

		for (auto block : rayColblocks) {
			for (int i = 0; i < block->GetAABBNum(); i++) {
				CVector3 hitPos;
				if (CMath::ColRayAndAABB(start, dir, block->GetAABB(i).min, block->GetAABB(i).max, &hitPos)) {
					float hitDistanceSq = (hitPos - start).LengthSq();
					if (hitDistanceSq <= CMath::Square(reyLength) && !(dir.Dot((hitPos - start)) < 0.0f)) {
						if (distance < 0.0f || distance >hitDistanceSq) {
							distance = hitDistanceSq;
							hitPair.first = hitPos;
							hitPair.second = block;
							hitAABBcenter = (block->GetAABB(i).min + block->GetAABB(i).max) / 2.0f;
							break;
						}
					}
				}
			}
		}

		if(hitPair.second != nullptr){
			if (return_hitPos) {
				*return_hitPos = hitPair.first;
			}
			if (return_hitNormal) {
				CVector3 normal = hitPair.first - hitAABBcenter;
				CVector3 normalAbs = normal; normalAbs.Abs();
				if (normalAbs.x > normalAbs.y && normalAbs.x > normalAbs.z) {
					normal = normal.x < 0.0f ? CVector3::AxisX()*-1.0f : CVector3::AxisX();
				}
				else if (normalAbs.y > normalAbs.x && normalAbs.y > normalAbs.z) {
					normal = normal.y < 0.0f ? CVector3::AxisY() * -1.0f : CVector3::AxisY();
				}
				else if (normalAbs.z > normalAbs.y && normalAbs.z > normalAbs.x) {
					normal = normal.z < 0.0f ? CVector3::AxisZ() * -1.0f : CVector3::AxisZ();
				}
				*return_hitNormal = normal;
			}
			return hitPair.second;
		}
	}

	return nullptr;
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

	//チャンクのライト情報初期化
	{
		IntVector3 pos = IntVector3::Zero();

		for (int ix = 0; ix < 2; ix++) {
			if (ix == 0) {
				pos.x = x * Chunk::WIDTH - 1;
			}
			else {
				pos.x = x * Chunk::WIDTH + Chunk::WIDTH;
			}
			for (int iy = 0; iy < Chunk::HEIGHT; iy++) {
				pos.y = iy;
				for (int iz = 0; iz < Chunk::WIDTH; iz++) {
					pos.z = z * Chunk::WIDTH + iz;

					char* light = GetLightData(pos);
					if (light && *light > 1) {
						LightUtil::SpreadLight(this, *light - 1, pos, IntVector3::Zero(), false);
					}
				}
			}
		}

		for (int ix = 0; ix < 2; ix++) {
			if (ix == 0) {
				pos.z = z * Chunk::WIDTH - 1;
			}
			else {
				pos.z = z * Chunk::WIDTH + Chunk::WIDTH;
			}
			for (int iy = 0; iy < Chunk::HEIGHT; iy++) {
				pos.y = iy;
				for (int iz = 0; iz < Chunk::WIDTH; iz++) {
					pos.x = x * Chunk::WIDTH + iz;

					char* light = GetLightData(pos);
					if (light && *light > 1) {
						LightUtil::SpreadLight(this, *light - 1, pos, IntVector3::Zero(), false);
					}
				}
			}
		}
	}

	//ファイルにチャンクがあれば、それを読む。
	ChunkFiler filer;
	filer.Read( *chunk );

	return chunk;
}

bool World::LoadChunk( int x, int z ){
	if( !IsExistChunk( x, z ) ){
		Chunk* chunk = CreateChunk( x, z );

		//ファイルにチャンクが存在しなかったか、存在はしたが生成が済んでない場合。
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );
		}

		//埋まったブロックを非表示にする。
		ChunkCulling( *chunk );

		if (!chunk->IsCalcedSkyLight()) {
			//スカイライトの計算を行う
			SkyLight skylight(this);
			skylight.CalcSkyLight(chunk);
		}

		return true;
	} 
	else{
		Chunk* chunk = GetChunk( x, z );

		//チャンクが存在はしたが生成が住んでない場合。
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );

			//埋まったブロックを非表示にする。
			ChunkCulling( *chunk );
		}

		if (!chunk->IsCalcedSkyLight()) {
			//スカイライトの計算を行う
			SkyLight skylight(this);
			skylight.CalcSkyLight(chunk);
		}

		return false;
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
			if( neighbor == nullptr || neighbor->GetIsOpacity() == false || neighbor->GetIsTransTexture() == true){//ブロックない or 透明ブロック
				doCulling = false;
				break;
			}
		}

		if( doCulling )b->SetIsDraw( false );
	} );
}

const Block* World::DamegeBlock( const CVector3& pos, EnTool toolType, int toolLevel ){
	//タイマーを0にする
	m_timer = 0.0f;
	int x = (int)std::floorf( pos.x );
	int y = (int)std::floorf( pos.y );
	int z = (int)std::floorf( pos.z );
	if (y == RandomMapMaker::m_minHeight)
	{
		return nullptr;
	}

	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	m_block = GetBlock(x, y, z);
	if (m_block == nullptr)
	{
		return nullptr;
	}
	//ダメージを算出
	int damege = 1;
	if (m_block->GetUsefulTool() == toolType) {
		damege = toolLevel*2;
	}
	//ブロックのHPを減らす
	m_block->ReduceHP(damege);
	//ブロックのHPが0以上ならこれで終わり
	if (m_block->GetHP() > 0)
	{
		m_blockPosX = x;
		m_blockPosY = y;
		m_blockPosZ = z;
		return m_block;
	}
	//ブロックが破壊された。
	else {
		//ドロップアイテムを生成する。
		m_block->DestroyedPlayer();
		//音出してます。
		SuicideObj::CSE* destroy;
		destroy = NewGO<SuicideObj::CSE>(L"Resource/soundData/block/blockdestroy.wav");
		destroy->Play();
	}
	//ブロックをポップ。
	{
		//ドロップアイテムを作成。
		DropItem* dropItem = DropItem::CreateDropItem( this, GetBlock( x, y, z )->GetDropItem() );
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

	return m_block;
}

void World::DestroyBlock(const IntVector3& pos) {
	if (pos.y <= 0 || pos.y >= Chunk::HEIGHT) {
		return;
	}

	Chunk* chunk = GetChunkFromWorldPos(pos.x, pos.z);
	if (!chunk) {
		return;
	}

	auto block = GetBlock(pos);
	if (block == nullptr)
	{
		return;
	}

	//ブロックが破壊された。
	{
		//ドロップアイテムを生成する。
		block->DestroyedPlayer();
		//音出してます。
		/*SuicideObj::CSE* destroy;
		destroy = NewGO<SuicideObj::CSE>(L"Resource/soundData/block/blockdestroy.wav");
		destroy->Play();*/
	}
	//ブロックをポップ。
	{
		//ドロップアイテムを作成。
		DropItem* dropItem = DropItem::CreateDropItem(this, block->GetDropItem());
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
		dropItem->SetPos(CVector3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f) * Block::WIDTH + addPos);
	}

	int x = Chunk::CalcInChunkCoord(pos.x);
	int z = Chunk::CalcInChunkCoord(pos.z);

	{
		Block* block = chunk->GetBlock(x, pos.y, z);
		if (block && block->GetBlockType() == enCube_Bedrock) {//岩盤は無理
			return;
		}
	}

	//破壊
	chunk->DeleteBlock(x, pos.y, z);
	AroundBlock({ (float)pos.x,(float)pos.y,(float)pos.z });
}

void World::DestroyBlockNoDrop(const IntVector3& pos) {
	if (pos.y <= 0 || pos.y >= Chunk::HEIGHT) {
		return;
	}

	Chunk* chunk = GetChunkFromWorldPos(pos.x, pos.z);
	if (!chunk) {
		return;
	}
	int x = Chunk::CalcInChunkCoord(pos.x);
	int z = Chunk::CalcInChunkCoord(pos.z);

	{
		Block* block = chunk->GetBlock(x, pos.y, z);
		if (block && block->GetBlockType() == enCube_Bedrock) {//岩盤は無理
			return;
		}
	}

	//破壊
	chunk->DeleteBlock(x, pos.y, z);
	AroundBlock({ (float)pos.x,(float)pos.y,(float)pos.z });
}

bool World::PlaceBlock( const CVector3& pos, std::unique_ptr<Block> block ){
	int x = (int)std::floorf( pos.x );
	int y = (int)std::floorf( pos.y );
	int z = (int)std::floorf( pos.z );
	if (y > maxHeight || y <= 0)
	{
		m_errorTimer = errorTime;
		return false;
	}

	//プレイヤーのいるところには衝突するブロック設置できない
	if (block->GetIsColision()) {
		const AABB& playerAABB = m_player->GetAABB();

		AABB blockAABB;
		blockAABB.min = CVector3(x * Block::WIDTH + Block::WIDTH / 2, y * Block::WIDTH, z * Block::WIDTH + Block::WIDTH / 2);
		blockAABB.max = blockAABB.min;
		blockAABB.min -= CVector3(Block::WIDTH / 2.0f, 0.0f, Block::WIDTH / 2.0f);
		blockAABB.max += CVector3(Block::WIDTH / 2.0f, Block::WIDTH, Block::WIDTH / 2.0f);

		if (CMath::ColAABBs(playerAABB.min, playerAABB.max, blockAABB.min, blockAABB.max)) {
			return false;
		}
	}

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
			pos2 += CVector3((float)block->GetMukiDir().x, (float)block->GetMukiDir().y, (float)block->GetMukiDir().z)*-1.0f;
		}
		else {
			pos2 += CVector3((float)block->GetMukiDir().x, (float)block->GetMukiDir().y, (float)block->GetMukiDir().z);
		}
		int x2 = (int)std::floorf(pos2.x);
		int y2 = (int)std::floorf(pos2.y);
		int z2 = (int)std::floorf(pos2.z);

		Chunk* chunk2 = GetChunkFromWorldPos(x2, z2);
		if (!chunk2) {
			chunk2 = CreateChunkFromWorldPos(x2, z2);
		}

		x2 = Chunk::CalcInChunkCoord(x2);
		z2 = Chunk::CalcInChunkCoord(z2);

		//2つおけるか?
		if (!chunk->CanPlaceBlock(x, y, z) || !chunk2->CanPlaceBlock(x2, y2, z2)) {
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
		chunk2->PlaceBlock(x2, y2, z2, std::move(block2));

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

		//2つおけるか?
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
			if (neighbor == nullptr || neighbor->GetIsOpacity() == false || neighbor->GetIsTransTexture() == true) {//ブロックない or 透明ブロック
				doNotCulling = true;
				break;
			}
		}

		block->SetIsDraw( doNotCulling );
	}
}

void World::SaveChunk()
{
	//全てのチャンクデータを保存する。
	for (auto itr = m_chunkMap.begin(); itr != m_chunkMap.end();) {

		Chunk& chunk = itr->second;
		const int x = chunk.GetX(), z = chunk.GetZ();

		ChunkFiler filer;
		filer.Write(chunk);
		itr = m_chunkMap.erase(itr);
	}
}