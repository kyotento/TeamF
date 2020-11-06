//! @file
#pragma once

//! @brief ���[���h�Ɋւ�������t�@�C���ɕۑ�����N���X�B
//! @author Takayama
class WorldInfoFile{
public:

	//! @brief �t�@�C������ǂ݂��݁B
	//! @retval false �t�@�C���ǂݍ��݂Ɏ��s�����B
	bool Read();

	//! @brief �t�@�C���ɏ������݁B
	void Write();

	//! @brief �V�[�h�l�����@���擾�B
	std::mt19937& GetSeedGenerator(){
		return m_random;
	}

	//! @brief ���ׂĂ̌��ɂȂ�V�[�h�l��ݒ�B
	void SetSeed( uint32_t seed ){
		m_info.randomSeed = seed;
	}

	//! @brief ���ׂĂ̌��ɂȂ�V�[�h�l�������_���ɐݒ�B
	void SetRandomSeed();

private:
	struct Info{
		uint32_t randomSeed;
	};

	Info m_info;

	std::mt19937 m_random;
};

