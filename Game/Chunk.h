//! @file
#pragma once
#include "Block.h"

/*! @brief World �N���X�͕ێ�����ׂ� Block �I�u�W�F�N�g�����̃N���X���Ƃɕ����ĕێ�����B
	@details #WIDTH * #HEIGHT * #WIDTH ��3�����z��Ńu���b�N��ێ�����B
	@author Takayama */
class Chunk{
public:

	Chunk(){}

	Chunk( const Chunk& c ) = delete;
	const Chunk& operator=( const Chunk& c ) = delete;

	Chunk( Chunk&& c )noexcept{
		m_chunkX = c.m_chunkX;
		m_chunkZ = c.m_chunkZ;
		std::move( &c.m_blockArray[0][0][0], &c.m_blockArray[WIDTH - 1][HEIGHT - 1][WIDTH - 1], &m_blockArray[0][0][0] );
	}
	const Chunk& operator=( Chunk&& c )noexcept{
		m_chunkX = c.m_chunkX;
		m_chunkZ = c.m_chunkZ;
		std::move( &c.m_blockArray[0][0][0], &c.m_blockArray[WIDTH - 1][HEIGHT - 1][WIDTH - 1], &m_blockArray[0][0][0] );
	}

	//! @brief �`�����N���̍��W�Ń��C�g�����擾
	char* GetLightData(int x, int y, int z) {
		return &m_lightData[x][y][z];
	}
	//! @brief �`�����N���̍��W�Ń��C�g�����擾
	char* GetLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetLightData(x, y, z);
	}

	//! @brief �`�����N���̍��W�ŃX�J�C���C�g�����擾
	char* GetSkyLightData(int x, int y, int z) {
		return &m_skyLightData[x][y][z];
	}
	//! @brief �`�����N���̍��W�ŃX�J�C���C�g�����擾
	char* GetSkyLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetLightData(x, y, z);
	}

	//! @brief �`�����N���̍��W�Ńu���b�N���擾
	Block* GetBlock( int x, int y, int z ){
		return m_blockArray[x][y][z].get();
	}
	//! @brief �`�����N���̍��W�Ńu���b�N���擾
	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		return GetBlock( x, y, z );
	}

	//! @brief �`�����N���̍��W�Ńu���b�N��ݒ�
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
		m_blockArray[x][y][z] = std::move( block );
		m_blockArray[x][y][z]->SetPos( CalcWorldCoordX( x ), y, CalcWorldCoordZ( z ) );
	}

	//! @brief �`�����N���̍��W�Ńu���b�N��ݒ�
	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		SetBlock( x, y, z, std::move( block ) );
	}

	//�`�����N�����W�Ńu���b�N��ݒu�A�v���C���[�p�B
	bool PlaceBlock(const int x, const int y, const int z, std::unique_ptr<Block> block)
	{
		
		if (m_blockArray[x][y][z] == nullptr) {
			m_blockArray[x][y][z] = std::move(block);
			m_blockArray[x][y][z]->SetPos(CalcWorldCoordX(x), y, CalcWorldCoordZ(z));
			return true;
		}
		return false;
	}
	
	//�u���b�N���폜 �`�����N���W�Ńu���b�N���폜�B
	void DeleteBlock(const int x, const int y, const int z) {
		m_blockArray[x][y][z].reset();
	}

	//! @brief �`�����N�����W���v�Z
	//! @param coord ���[���h���W��x��z
	//! @return �`�����N�����W��x��z
	static int CalcInChunkCoord( int coord ){
		if( coord < 0 ){
			coord -= Chunk::WIDTH - 1;
			return ( Chunk::WIDTH - 1 ) + coord % Chunk::WIDTH;
		}
		return coord % Chunk::WIDTH;
	}

	//! @brief ���[���h���W��X�l���v�Z
	int CalcWorldCoordX( int x ){
		return x + m_chunkX * Chunk::WIDTH;
	}

	//! @brief ���[���h���W��Z�l���v�Z
	int CalcWorldCoordZ( int z ){
		return z + m_chunkZ * Chunk::WIDTH;
	}

	//! @brief �`�����N1����1�Ƃ����Ƃ��̂��̃`�����N�̍��W��ݒ�B
	void SetChunkPos( int x, int z ){
		m_chunkX = x; m_chunkZ = z;
	}

	//! @brief �`�����N���W��X�B
	int GetX() const{
		return m_chunkX;
	}
	//! @brief �`�����N���W��Z�B
	int GetZ() const{
		return m_chunkZ;
	}

	//! @brief �u���b�N�z��̐擪�A�h���X���擾�B
	const auto& Data() const{
		return m_blockArray;
	}

	//! @brief �u���b�N�z��̐擪�A�h���X���擾�B
	auto& Data(){
		return m_blockArray;
	}

	//! @brief �n�`���������t���O�𗧂Ă�B
	void SetGenerated(){
		m_isGenerated = true;
	}

	//! @brief �n�`�������������Ă��邩��Ԃ��B
	bool IsGenerated() const{
		return m_isGenerated;
	}

	//! @brief �`�����N��x,z�����̒����B
	static constexpr int WIDTH = 16;
	//! @brief �`�����N��y�����̒����B
	static constexpr int HEIGHT = 64;

private:
	int m_chunkX = 0;
	int m_chunkZ = 0;

	//���C�g���
	char m_lightData[WIDTH][HEIGHT][WIDTH] = {};
	char m_skyLightData[WIDTH][HEIGHT][WIDTH] = {};

	std::unique_ptr<Block> m_blockArray[WIDTH][HEIGHT][WIDTH];

	bool m_isGenerated = false;
};

