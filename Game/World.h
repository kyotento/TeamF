//! @file
#pragma once
#include "Chunk.h"
#include "RandomMapMaker.h"
#include "WorldInfoFile.h"

class Entity;
class Player;
class DropItem;

//! @brief Block �I�u�W�F�N�g��ێ����郏�[���h
//! @author Takayama
class World : public IGameObject{
public:
	World();
	~World();

	//! @brief �X�V�֐��B�`�����N���X�g���[�W�ɑޔ������鏈��������B
	void PostUpdate() override;

	//! @brief Player ���Z�b�g����B
	void SetPlayer( Player* player ){
		m_player = player;
	}

	Player* GetPlayer();

	//! @brief Entity �����[���h�ɐ�������B�����͑Ώۂ̃R���X�g���N�^�B
	template<class T, class... TArgs>
	T*  CreateEntity( TArgs... ctorArgs ){
		T* newEntity = NewGO<T>( ctorArgs... );
		newEntity->SetWorld( this );
		m_entities.insert( newEntity );
		return newEntity;
	}

	//! @brief Entity �����[���h�����菜���BDeleteGO�͂��Ȃ��B
	void RemoveEntity( Entity* entity ){
		m_entities.erase( entity );
	}

	//! @brief �`�����N��ǂݍ��ދ������擾�B
	//! @details ���̒l��2�{��ӂƂ��鐳���`���ǂݍ��ݔ͈́B
	int GetChunkLoadRange(){
		return m_chunkLoadRange;
	}
	
	//! @brief �w��ʒu�̃u���b�N���擾�B
	//! @details ���W�̓u���b�N�P��
	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		return GetBlock( x, y, z );
	}
	Block* GetBlock( int x, int y, int z );

	/// <summary>
	/// �w���AABB�t�߂̃u���b�N���擾
	/// </summary>
	/// <param name="aabbmin">AABB�̏��������̃��[���h���W</param>
	/// <param name="aabbmax">AABB�̑傫�����̃��[���h���W</param>
	/// <param name="return_blocks">���̃x�N�^�[�ɔ͈͓��̃u���b�N���ǉ������</param>
	void GetBlocks(CVector3 aabbmin, CVector3 aabbmax, std::vector<Block*>& return_blocks);

	//! @brief �w��ʒu�Ƀu���b�N���Z�b�g�B
	//! @details ���W�̓u���b�N�P��
	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		SetBlock( x, y, z, std::move(block) );
	}
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block );

	//���[���h���W��Block::WIDTH�Ŋ��������̂����ɁB
	//�u���b�N��ݒu�A�v���C���[�p�B
	//�ݒu�ɐ���������true�B
	bool PlaceBlock(const CVector3& pos, std::unique_ptr<Block> block);

	//���[���h���W��Block::WIDTH�Ŋ��������̂����ɁB
	//�u���b�N��j��A�v���C���[�p�B
	void DeleteBlock(const CVector3& pos);

	//��̃u���b�N�̎���̃u���b�N�̃J�����O����������B
	void AroundBlock(const CVector3& pos);

	//! @brief �w��ʒu�̃��C�g�����擾�B
	//! @details ���W�̓u���b�N�P��
	char* GetLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetLightData(x, y, z);
	}
	char* GetLightData(int x, int y, int z);

	//! @brief �w��ʒu�̃X�J�C���C�g�����擾�B
	//! @details ���W�̓u���b�N�P��
	char* GetSkyLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetSkyLightData(x, y, z);
	}
	char* GetSkyLightData(int x, int y, int z);

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
	//! @brief�G���e�B�e�B���擾
	//! @return �G���e�B�e�B�̔z��B
	std::unordered_set<Entity*>& GetEntities()
	{
		return m_entities;
	}
private:
	//! @brief �`�����N�����[�h�B���[�h�ς݂Ȃ牽�����Ȃ��B
	void LoadChunk(int x, int z);

	//! @brief �`�����N���Ƃɖ��܂��Ă���u���b�N���\���ɂ���
	void ChunkCulling(Chunk& chunk);

	RandomMapMaker m_mapMaker;
	std::map<std::pair<int, int>, Chunk> m_chunkMap;

	Player* m_player = nullptr;

	//! �ǂݍ��ݔ͈͂��w��B�`�����N�P�ʁB
	int m_chunkLoadRange = 1;

	//! �v���C���[�𒆐S�Ƃ����A�R���W�������L���������͈́B�u���b�N�P�ʁB
	int m_collisionEnableRange = 2;
	std::unordered_set<IntVector3> m_activeCollisions;

	//! �G���e�B�e�B(�u���b�N�ȊO�̓�����)�����Ă����z��B
	std::unordered_set<Entity*> m_entities;

	//!�V�[�h�l�Ȃǂ̏���ۑ��B
	WorldInfoFile infoFile;
};

