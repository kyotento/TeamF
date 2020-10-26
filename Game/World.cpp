#include "stdafx.h"
#include "World.h"
#include "ChunkFiler.h"
#include "Player.h"
#include "IntRect.h"

World::World(){
	m_mapMaker.Init( this );
}

void World::PostUpdate(){

	{
		const CVector3 pPosV = m_player->GetPos() / Block::WIDTH;

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
	AddEntity( player );
	if( recursive )
		player->SetWorld( this, false );
}

Block * World::GetBlock( int x, int y, int z ){
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
	chunkRect.for_each( [&](int x, int y, int z){
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
