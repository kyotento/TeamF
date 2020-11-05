//! @file
#pragma once

//! @brief ワールドに関する情報をファイルに保存するクラス。
//! @author Takayama
class WorldInfoFile{
public:

	//! @brief ファイルから読みこみ。
	//! @retval false ファイル読み込みに失敗した。
	bool Read();

	//! @brief ファイルに書き込み。
	void Write();

	//! @brief シード値生成機を取得。
	std::mt19937& GetSeedGenerator(){
		return m_random;
	}

	//! @brief すべての元になるシード値を設定。
	void SetSeed( uint32_t seed ){
		m_info.randomSeed = seed;
	}

	//! @brief すべての元になるシード値をランダムに設定。
	void SetRandomSeed();

private:
	struct Info{
		uint32_t randomSeed;
	};

	Info m_info;

	std::mt19937 m_random;
};

