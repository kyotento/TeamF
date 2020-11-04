#include "stdafx.h"
#include "CreateOre.h"
#include "Perlin.h"
#include "World.h"
#include "BlockFactory.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include "World.h"

namespace {
	std::mt19937 random((std::random_device())());
}

void CreateOre::Awake(Chunk& chunk)
{
	int seed = 101;

	//シード値を初期化
	m_seedX = random() % seed;
	m_seedY = random() % seed;
	m_seedZ = random() % seed;

	//マップの広さ持ってきて

	m_maxHeight = m_mapMaker->m_maxHeight;
	m_minHeight = m_mapMaker->m_minHeight;
	m_startX = chunk.CalcWorldCoordX(0);
	m_startZ = chunk.CalcWorldCoordZ(0);
	m_width = chunk.CalcWorldCoordX(Chunk::WIDTH);
	m_depth = chunk.CalcWorldCoordZ(Chunk::WIDTH);

	//鉱石のデータを初期化
	m_oreDataList[enCube_IronOre] = OreData();
	m_oreDataList[enCube_IronOre].s_oreMaxHeight = 7;
	m_oreDataList[enCube_IronOre].s_oreMinHeight = 3;
	m_oreDataList[enCube_IronOre].s_oreDiffHeight = m_oreDataList[enCube_IronOre].s_oreMaxHeight - m_oreDataList[enCube_IronOre].s_oreMinHeight;
	m_oreDataList[enCube_IronOre].s_oreMaxNumber = 2;
	m_oreDataList[enCube_IronOre].s_oreWidth = 2;
	m_oreDataList[enCube_IronOre].s_oreDepth = 2;
	m_oreDataList[enCube_IronOre].s_oreHeight = 2;
	m_oreDataList[enCube_IronOre].s_prob = 50;
	m_oreDataList[enCube_IronOre].s_oreDivideRand = (m_width - m_startX) * (m_depth - m_startZ) * m_oreDataList[enCube_IronOre].s_oreDiffHeight;

	m_oreDataList[enCube_GoldOre] = OreData();
	m_oreDataList[enCube_GoldOre].s_oreMaxHeight = 4;
	m_oreDataList[enCube_GoldOre].s_oreMinHeight = 0;
	m_oreDataList[enCube_GoldOre].s_oreDiffHeight = m_oreDataList[enCube_GoldOre].s_oreMaxHeight - m_oreDataList[enCube_GoldOre].s_oreMinHeight;
	m_oreDataList[enCube_GoldOre].s_oreMaxNumber = 1;
	m_oreDataList[enCube_GoldOre].s_oreWidth = 2;
	m_oreDataList[enCube_GoldOre].s_oreDepth = 2;
	m_oreDataList[enCube_GoldOre].s_oreHeight = 2;
	m_oreDataList[enCube_GoldOre].s_prob = 30;
	m_oreDataList[enCube_GoldOre].s_oreDivideRand = (m_width - m_startX) * (m_depth - m_startZ) * m_oreDataList[enCube_GoldOre].s_oreDiffHeight;

	m_oreDataList[enCube_Soil] = OreData();
	m_oreDataList[enCube_Soil].s_oreMaxHeight = 7;
	m_oreDataList[enCube_Soil].s_oreMinHeight = 0;
	m_oreDataList[enCube_Soil].s_oreDiffHeight = m_oreDataList[enCube_GoldOre].s_oreMaxHeight - m_oreDataList[enCube_GoldOre].s_oreMinHeight;
	m_oreDataList[enCube_Soil].s_oreMaxNumber = 3;
	m_oreDataList[enCube_Soil].s_prob = 53;
	m_oreDataList[enCube_Soil].s_oreDivideRand = (m_width - m_startX) * (m_depth - m_startZ) * m_oreDataList[enCube_GoldOre].s_oreDiffHeight;
	m_oreDataList[enCube_Soil].s_atten = 16;
	m_oreDataList[enCube_Soil].s_isDig = false;

	//生成した鉱石の個数を計る
	int oreNumber = 0;
	for (int x = m_startX; x < m_width; x++) {
		for (int z = m_startZ; z < m_depth; z++) {
			for (int y = m_maxHeight; y >= m_minHeight; y--) {
				//(⌒,_ゝ⌒)鉱石を生成するかどうか判断するんですわぁ・・・
				DetermineGenerate(x, y, z);
			}
		}
	}
}

void CreateOre::DetermineGenerate(const int x, const int y, const int z)
{
	//データのリストをまわして
	for (auto& itr : m_oreDataList) {
		//高さがあってなかった、あるいは一定以上生成していたら以下処理しない
		if (itr.second.s_oreMinHeight >= y || y >= itr.second.s_oreMaxHeight || itr.second.s_oreNumber >= itr.second.s_oreMaxNumber)
			continue;
		
		//0で割るとエラー起きるのでスルー
		if (itr.second.s_oreDivideRand == 0)
			continue;
		//乱数を生成して。
		int r = random() % itr.second.s_oreDivideRand;
	
		//乱数が範囲内だったら鉱石生成
		if (r <= itr.second.s_oreMaxNumber) {
			m_world->SetBlock(x, y, z, BlockFactory::CreateBlock(itr.first));
			itr.second.s_oreNumber++;
			//周りにも鉱石生成
			CreateAround(x, y, z, itr.first, itr.second.s_isDig, itr.second.s_prob); 
			return;
		}
	}
}

void CreateOre::GeneratePerlinNoise(const int x, const int y, const int z)
{
	//データのリストをまわして
	for (auto& itr : m_perlinDataList) {
		//高さがあってなかった、あるいは一定以上生成していたら以下処理しない
		if (itr.second.s_perlinMinHeight >= y || y >= itr.second.s_perlinMaxHeight || itr.second.s_perlinNumber >= itr.second.s_perlinMaxNumber)
			continue;
		//ノイズを生成して
		float xSample = (x + m_seedX) / itr.second.s_relief;
		float ySample = (y + m_seedY) / itr.second.s_relief;
		float zSample = (z + m_seedZ) / itr.second.s_relief;

		float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);
		//ノイズが一定以内なら生成する
		if (itr.second.s_minNoise <= noise && noise <= itr.second.s_maxNoise) {
			//m_world->SetBlock(x, y, z, BlockFactory::CreateBlock(itr.first), );
			itr.second.s_perlinNumber++;
		}
	}
}

void CreateOre::CreateAround(const int x, const int y, const int z, EnCube enCube, bool isDig, int p)
{
	//斜めに生成しない
	if (!isDig) {
		std::vector<CVector3> posList;
		posList.push_back(CVector3(-1, 0, 0));
		posList.push_back(CVector3(1, 0, 0));
		posList.push_back(CVector3(0, -1, 0));
		posList.push_back(CVector3(0, 1, 0));
		posList.push_back(CVector3(0, 0, -1));	
		posList.push_back(CVector3(0, 0, 1));

		//ちょっとコメント書くのめんどい
		for (int i = 0; i < posList.size(); i++) {
			int xx = x + int(posList[i].x);
			int yy = y + int(posList[i].y);
			int zz = z + int(posList[i].z);
			if (m_width < xx || m_maxHeight < yy || yy < m_minHeight || m_depth < zz)
				continue;
			int r = random() % 100;
			//乱数が範囲ないなら生成
			if (r < p) {
				m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube));
				if (enCube == enCube_Soil) {
					int prob = p - m_oreDataList[enCube].s_atten;
					if (prob <= 0) {
						continue;
					}
					//更に周りにも生成していく
					CreateAround(xx, yy, zz, enCube, m_oreDataList[enCube].s_isDig, prob);
				}
			}
		}
		return;
	}

	//斜めに生成する
	for (int xx = x; xx < x + m_oreDataList[enCube].s_oreWidth; xx++) {
		for (int yy = y; yy < y + m_oreDataList[enCube].s_oreHeight; yy++) {
			for (int zz = z; zz < z + m_oreDataList[enCube].s_oreDepth; zz++) {
				if (m_width < xx || m_maxHeight < yy || yy < m_minHeight || m_depth < zz)
					continue;
				int r = random() % 100;
				//乱数が範囲内なら生成
				if (r < p) {
					m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube));
					if (enCube == enCube_Soil) {
						int prob = p - m_oreDataList[enCube].s_atten;
						if (prob <= 0) {
							continue;
						}
						CreateAround(xx, yy, zz, enCube,m_oreDataList[enCube].s_isDig,prob);
					}
				}
			}
		}
	}
}