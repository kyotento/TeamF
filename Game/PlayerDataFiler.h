//! @file
#pragma once

class Player;

//! @brief �v���C���[�̏����t�@�C���ɕۑ�����B
class PlayerDataFiler
{
public:
	void Load(Player* player);
	void Save(const Player* player);

	bool IsLoadSuccess(){
		return m_loadSuccess;
	}

private:
	static const int16_t VERSION;

	bool m_loadSuccess = false;
};

