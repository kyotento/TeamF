#include "stdafx.h"
#include "World.h"
#include "ChunkFiler.h"
#include "Player.h"
#include "IntRect.h"
#include "BiomeManager.h"
#include "DropItem.h"

World::World(){
	bool result = infoFile.Read();

	if( result == false ){
		//�t�@�C�������݂��Ȃ��ꍇ�ɐV�����쐬����B
		infoFile.SetRandomSeed();
		infoFile.Write();
	}

	//�V�[�h�l�����ꂼ��ɐݒ肷��B
	BiomeManager::GetInstance().GenerateSeed( infoFile.GetSeedGenerator() );
	m_mapMaker.Init( this, infoFile.GetSeedGenerator() );
}

void World::PostUpdate(){

	Player* player = GetPlayer();
	if( player ){
		const CVector3 pPosV = player->GetPos() / Block::WIDTH;

		//�`�����N�ǂݍ��ݔ͈͐����`�B
		IntRect chunkSquare = IntRect::CreateWithCenter(
			IntVector3( CalcChunkCoord( (int)pPosV.x ), 0, CalcChunkCoord( (int)pPosV.z ) ),
			{ m_chunkLoadRange, 1, m_chunkLoadRange } );

		//�ǂݍ��܂�Ă��Ȃ��`�����N��ǂݍ��ށB
		chunkSquare.for_each( [&]( int x, int, int z ){
			LoadChunk( x, z );
		} );

		//�͈͊O�̃`�����N�������o���ă�������������B
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
		//�͈͓��̃u���b�N�̃R���W������L��������B�͈͊O�͖���������B

		//���������[�v
		for( auto itr = m_activeCollisions.begin(); itr != m_activeCollisions.end(); ){

			const IntVector3& p = *itr;
			bool disable = true;

			//�S�G���e�B�e�B�����[�v
			for( Entity* e : m_entities ){

				const IntRect eCube = IntRect::CreateWithCenter(
					IntVector3( e->GetPos() / Block::WIDTH ), m_collisionEnableRange );

				if( eCube.IsInclude( p ) ){
					//1�̂ł��G���e�B�e�B���͈͓��Ȃ疳�������Ȃ��B
					itr++;
					disable = false;
					break;
				}

			}

			if( disable ){

				//�N�͈͓̔��ł��Ȃ���Ζ������B
				Block* b = GetBlock( p.x, p.y, p.z );
				if( b )b->DisableCollision();
				itr = m_activeCollisions.erase( itr );
			}
		}

		//�L�������[�v�B
		for( Entity* e : m_entities ){

			const IntRect eCube = IntRect::CreateWithCenter(
				IntVector3( e->GetPos() / Block::WIDTH ), m_collisionEnableRange );

			//�e�G���e�B�e�B�̋߂��̃u���b�N�����[�v
			eCube.for_each( [&]( int x, int y, int z ){

				Block* b = GetBlock( x, y, z );

				if( b && !b->IsCollisionEnabled() ){
					//�L�������ėL���Z�b�g�ɓ����B
					b->EnableCollision();
					m_activeCollisions.emplace( x, y, z );
				}

			} );
		}
	}
}

void World::SetPlayer( Player* player, bool recursive ){
	m_player = player;
	if( recursive )
		player->SetWorld( this, false );
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

void World::SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );
	chunk->SetBlock( x, y, z, std::move( block ) );
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

	//�t�@�C���Ƀ`�����N������΁A�����ǂށB
	ChunkFiler filer;
	filer.Read( *chunk );

	return chunk;
}

void World::LoadChunk( int x, int z ){
	if( !IsExistChunk( x, z ) ){
		Chunk* chunk = CreateChunk( x, z );

		//�t�@�C������ǂݍ��ށB
		ChunkFiler filer;
		bool readResult = filer.Read( *chunk );

		//�t�@�C���Ƀ`�����N�����݂��Ȃ��������A���݂͂������������ς�łȂ��ꍇ�B
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );
		}

		//���܂����u���b�N���\���ɂ���B
		ChunkCulling( *chunk );
	} else{
		Chunk* chunk = GetChunk( x, z );
		//�`�����N�����݂͂������������Z��łȂ��ꍇ�B
		if( !chunk->IsGenerated() ){
			m_mapMaker.GenerateChunk( *chunk );
			ChunkCulling( *chunk );
		}
	}
}

void World::ChunkCulling( Chunk& chunk ){

	//6���������߂��z��B
	static constexpr IntVector3 vArray[]{
		{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}
	};

	//�`�����N�̒����́B
	IntRect chunkRect( { 0, 0, 0 }, { chunk.WIDTH , chunk.HEIGHT, chunk.WIDTH } );

	//�`�����N���̑S�u���b�N�����[�v�B
	chunkRect.for_each( [&]( int x, int y, int z ){
		Block* b = chunk.GetBlock( x, y, z );

		if( !b ){
			return;
		}

		//�`�����N���W���烏�[���h���W�֕ϊ��B
		int wx = chunk.CalcWorldCoordX( x );
		int wz = chunk.CalcWorldCoordZ( z );

		bool doCulling = true;

		//�㉺���E�O��𒲂ׂ郋�[�v�B
		for( const IntVector3& v : vArray ){
			if( y + v.y == -1 || y + v.y == chunk.HEIGHT ){
				continue;
			}

			if( GetBlock( wx + v.x, y + v.y, wz + v.z ) == nullptr ){
				doCulling = false;
				break;
			}
		}

		if( doCulling )b->SetIsDraw( false );
	} );
}

void World::DeleteBlock( const CVector3& pos ){
	int x = (int)std::floorf( pos.x );
	int y = (int)std::floorf( pos.y );
	int z = (int)std::floorf( pos.z );

	Chunk* chunk = GetChunkFromWorldPos( x, z );

	if( !chunk ){
		chunk = CreateChunkFromWorldPos( x, z );
	}

	//�u���b�N���|�b�v�B
	{
		DropItem* dropItem = NewGO<DropItem>( this );		//�h���b�v�A�C�e���N���X���擾�B

		dropItem->SetEnCube( GetBlock( x, y, z )->GetBlockType() );		//�u���b�N�̎�ނ����B
		dropItem->SetPos( CVector3( x + 0.5f, y + 0.5f, z + 0.5f ) );
		dropItem->Drop();
	}
	x = Chunk::CalcInChunkCoord( x );
	z = Chunk::CalcInChunkCoord( z );

	chunk->DeleteBlock( x, y, z );
	AroundBlock( pos );
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

	if( !chunk->PlaceBlock( x, y, z, std::move( block ) ) ){
		return false;
	}
	AroundBlock( pos );

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

			if( GetBlock( pos3 ) == nullptr ){
				doNotCulling = true;
				break;
			}
		}

		block->SetIsDraw( doNotCulling );
	}
}