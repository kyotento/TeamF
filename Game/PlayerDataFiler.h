//! @file
#pragma once

class Player;

//! @brief プレイヤーの情報をファイルに保存する。
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

class PlayerConfigDataFiler
{
public:
	void Load(Player* player);
	void Save(const Player* player);

	bool IsLoadSuccess() {
		return m_loadSuccess;
	}

private:
	static constexpr int16_t VERSION = 1;

	bool m_loadSuccess = false;
};