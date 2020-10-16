//! @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"

//! @brief Block �I�u�W�F�N�g��ێ����郏�[���h
//! @author Takayama
class World{
public:

	//! @brief ���[���h�𐶐�����B 
	void Generate(){
		m_mapMaker.Init(this);
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

	//=========�`�����N�̎擾�Ɋւ���֐��B==============

	//! @brief �`�����N���擾�B
	//! @param x,z �`�����N���W�B
	//! @return �`�����N�����݂��Ȃ��ꍇ�Anullptr�B
	Chunk* GetChunk( int x, int z );

	//! @brief �`�����N�𐶐����A�擾�B 
	//! @param x,z �`�����N���W
	//! @return ���łɑ��݂���ꍇ�͑��݂���`�����N��Ԃ��B
	Chunk* CreateChunk( int x, int z );

	//! @brief �`�����N�����݂��邩���ׂ�B
	//! @param x,z �`�����N���W
	bool IsExistChunk( int x, int z ){
		return m_chunkMap.count( std::make_pair( x, z ) ) != 0;
	}
	

	//==========�`�����N�̎擾�Ɋւ���֐��̃��[���h���W�ŁB==============

	//! @brief #GetChunk(int,int) �̃��[���h���W�ŁB
	Chunk* GetChunkFromWorldPos( int x, int z ){
		return GetChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//! @brief #CreateChunk(int,int) �̃��[���h���W�ŁB
	Chunk* CreateChunkFromWorldPos( int x, int z ){
		return CreateChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//! @brief #IsExistChunk(int,int) �̃��[���h���W�ŁB
	bool IsExistChunkFromWorldPos( int x, int z ){
		return m_chunkMap.count( std::make_pair( CalcChunkCoord(x), CalcChunkCoord(z) ) ) != 0;
	}



	//! @brief�`�����N���W���v�Z
	//! @param ���[���h���W��x��z�B
	//! @return �`�����N���W��x��z�B
	static int CalcChunkCoord( int num ){
		if( num < 0 )num -= Chunk::WIDTH - 1;
		return num / Chunk::WIDTH;
	}

	void Test(const CVector3& pos);
private:
	//! @brief ���[���h������ɖ��܂��Ă���u���b�N���\���ɂ���B
	void AllChunkCulling();
	//! @brief �`�����N���Ƃɖ��܂��Ă���u���b�N���\���ɂ���
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapMaker;
	std::map<std::pair<int, int>, Chunk> m_chunkMap;
};

