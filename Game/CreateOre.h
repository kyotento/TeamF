#pragma once
#include "BlockType.h"
class World;
class RandomMapMaker;
//鉱石系統生成のためのデータ
struct OreData {
	//最大の高さ
	int s_oreMaxHeight = 0;
	//最小の高さ
	int s_oreMinHeight = 0;
	//↑の高さの差分
	int s_oreDiffHeight = 0;
	//最大数
	int s_oreMaxNumber = 0;
	//ある一点に生成した鉱石の数
	int s_oreNumber = 0;
	//塊の大きさ
	int s_oreWidth = 0;
	int s_oreDepth = 0;
	int s_oreHeight = 0;
	//生成されるパーセンテージ
	int s_prob = 0;
	//乱数割るやつ
	int s_oreDivideRand = 1;
	//減衰値(石炭用)
	int s_atten = 0;
	//周りに生成する時に斜めに生成するか？
	bool s_isDig = true;
};

//パーリンノイズでやろうとしたデータ
struct PerlinData {
	//最大の高さ
	int s_perlinMaxHeight = 0;
	//最小の高さ
	int s_perlinMinHeight = 0;
	//最大数
	int s_perlinMaxNumber = 0;
	//生成した鉱石の数
	int s_perlinNumber = 0;
	//最小のノイズ値
	float s_minNoise = 0.f;
	//最大のノイズ値
	float s_maxNoise = 0.f;
	//起伏の激しさ
	float s_relief = 0.0f;
};

class Chunk;

class CreateOre
{
public:
	//鉱石を生成するぞ
	void Awake(Chunk& chunk);
	//ワールドを設定
	void SetWorld(World* world)
	{
		m_world = world;
	}
	//マップメーカーを設定
	void SetMapMaker(RandomMapMaker* mapMaker)
	{
		m_mapMaker = mapMaker;
	}
private:
	//ある一点に鉱石を生成するかどうか判断する
	void DetermineGenerate(const int x, const int y, const int z);
	//ある一点の周りに鉱石を生成する
	void CreateAround(const int x, const int y, const int z, EnCube enCube, bool isDig,int p = 0);
	//今は使ってないパーリンノイズで生成する
	void GeneratePerlinNoise(const int x, const int y, const int z);
private:
	World* m_world = nullptr;			//ワール
	RandomMapMaker* m_mapMaker = nullptr;		//マップメーカー
	std::map<EnCube, OreData> m_oreDataList;
	std::map<EnCube, PerlinData> m_perlinDataList;
	int m_seedX, m_seedY, m_seedZ = 0;
	int m_width, m_depth, m_maxHeight, m_minHeight;			//マップの広さ。
	int m_startX, m_startZ;    //マップのスタート値。
};

