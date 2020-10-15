/// @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"

/// @brief Block �I�u�W�F�N�g��ێ����郏�[���h
/// @author Takayama
class World{
public:

	/// @brief ���[���h�𐶐�����B 
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

	/// @brief �`�����N���擾�B
	/// @param x,z �`�����N���W
	/// @throw std::out_of_range 
	Chunk* GetChunk( int x, int z );

	/// @brief �`�����N�𐶐����A�擾�B 
	/// @details ���łɑ��݂���ꍇ�͑��݂���`�����N��Ԃ��B
	/// @param x,z �`�����N���W
	Chunk* CreateChunk( int x, int z );
	
	//@brief #GetChunk(int,int) �̃��[���h���W�ŁB
	Chunk* GetChunkFromWorldPos( int x, int z ){
		return GetChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	//@brief #CreateChunk(int,int) �̃��[���h���W�ŁB
	Chunk* CreateChunkFromWorldPos( int x, int z ){
		return CreateChunk( CalcChunkCoord( x ), CalcChunkCoord( z ) );
	}

	/// @brief�`�����N���W���v�Z
	static int CalcChunkCoord( int num ){
		if( num < 0 )num -= Chunk::WIDTH - 1;
		return num / Chunk::WIDTH;
	}

	void Test(const CVector3& pos);
private:
	/// @brief ���[���h������ɖ��܂��Ă���u���b�N���\���ɂ���B
	void AllChunkCulling();
	/// @brief �`�����N���Ƃɖ��܂��Ă���u���b�N���\���ɂ���
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapmMaker;
	std::map<int, std::map<int, Chunk>> m_chunkMap;
};

