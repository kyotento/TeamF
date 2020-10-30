#pragma once

enum enBiome{
	enBiome_None = -1,		//無効なバイオーム
	enBiome_Forest,			//森林。
	enBiome_Tundra,			//ツンドラ。
	enBiome_Plains,			//平原。
	enBiome_Desert,			//砂漠。
};

class BiomeManager
{
private:
	BiomeManager();
	~BiomeManager() {}
public:
	//シングルトン。
	static BiomeManager& GetInstance()
	{
		static BiomeManager instance;
		return instance;
	}
	//座標によりどのバイオームに属するか決定する。
	enBiome DecideBiome(const int x,const int y,const int z);
private:
	int m_seedX, m_seedZ, m_seedY;		//シード値。
	int m_seedX2, m_seedZ2, m_seedY2;
};

static inline BiomeManager& GetBiomeManager()
{
	return BiomeManager::GetInstance();
}

