#pragma once

class Block;
class World;
class Chunk;

class RandomMapMaker
{
public:
	//初期化
	void Init( World* world, std::mt19937& seedGenerator );

	//! @brief チャンクの地形を生成する。
	//! @param[in,out] chunk このチャンクに生成する。
	void GenerateChunk( Chunk& chunk );
	static const int m_maxHeight = 16;			//マップの最大の高さ
	static const int m_minHeight = 0;
private:
	//Y座標をきめたいわけ。
	float SetY(const CVector3& pos);
	//木を生やす。
	bool Tree(const int x, const int y, const int z);
	//ゾンビをつくる。
	void CreateZombie(const int x, const int y, const int z);
	
private:

	float m_seedX, m_seedZ, m_seedY;
	
	//マップサイズは撤去されました。チャンク読み込み範囲を変えるには、
	//Worldのm_chunkLoadRangeを変更してください。

	
	float m_relief = 20.f;			//起状の激しさ
	float m_mapSize = 1.f;			//マップの大きさ


	const int m_stoneMaxHeight = 3;
	const int m_stoneMinHeight = 0;
	const int m_OreMaxHeight = 2;
	const int m_OreMinHeight = 0;

	World* m_world = nullptr;
	float m_seedX2, m_seedZ2, m_seedY2;
	float m_relief2 = 2.5f;			//起状の激しさ
	float m_relief3 = 1000.0f;
};

