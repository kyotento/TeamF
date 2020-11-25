#pragma once

enum enBiome{
	enBiome_None = -1,		//�����ȃo�C�I�[��
	enBiome_Forest,			//�X�сB
	enBiome_Tundra,			//�c���h���B
	enBiome_Plains,			//�����B
	enBiome_Desert,			//�����B
};

class BiomeManager
{
private:
	BiomeManager();
	~BiomeManager() {}
public:
	//�V���O���g���B
	static BiomeManager& GetInstance()
	{
		static BiomeManager instance;
		return instance;
	}
	//���W�ɂ��ǂ̃o�C�I�[���ɑ����邩���肷��B
	enBiome DecideBiome(const int x,const int y,const int z);

	//! @brief �V�[�h�l�𗐐������킩��ݒ肷��B
	void GenerateSeed( std::mt19937& generater );
private:
	int m_seedX, m_seedZ;		//�V�[�h�l�B
	int m_seedX2, m_seedZ2;
};

static inline BiomeManager& GetBiomeManager()
{
	return BiomeManager::GetInstance();
}
