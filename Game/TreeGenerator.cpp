#include "stdafx.h"
#include "TreeGenerator.h"
#include "BlockFactory.h"
#include "Perlin.h"
#include "World.h"
#include "RandomMapMaker.h"

namespace {
	//ここから木の幹に関する変数。
	//パーリンノイズでノイズを生成してこのmin～maxまでの値だったら。
	//木を生成します。
	float minNoiseGenerateTree = 0.6f;
	float maxNoiseGenerateTree = 0.607f;
	//木の原木の最小の長さと最大の長さ。
	int minLengthTreeTrunk = 5;
	int maxLengthTreeTrunk = 7;

	//ここから葉の生成に関する変数。
	//どこから葉を生やすか、一番上の木の幹からどれだけ下にいくか。
	int minPositionLeaf = 3;
	//葉の生成パターン、多くすれば葉の形が増える。
	//この数値の3乗が葉の生成パターンになる、多分。
	int numberTreeGenerationPatterns = 12;
	//葉が生成される範囲のパラメータ。
	//葉の高さ。
	int leafHeight = 4;
	//葉の広さ。
	int leafWidth = 3;
	int leafDepth = 3;
	//この値を大きくしたら葉がより多く生成されるようになる。
	float differenceNoiseGenerationLeaf = 0.0015f;
	//葉を生成を判断する際に、ノイズ生成の時にY座標に加算する値。
	//この値を大きくするとノイズが大きく変動し、Y座標に葉が生成されにくくなる。
	int valueAddYDeterminingLeafGeneration = 15;

	//ここからは弄らない方がいいかも。
	//ノイズに乗算。
	float noiseMultiply = 100.f;
	int noiseMultiply2 = 10000;
	float seed = 13;
}

void TreeGenerator::GenerateTree(const int x, const int y, const int z)
{
	//木が生成されたら。
	if (GenerateWood(x, y, z))
	{
		//葉も生成していく。
		GenerateLeaf(x, y, z);
	}
}

bool TreeGenerator::GenerateWood(const int x, const int y, const int z)
{
	//同じマップを生成しないようにシード生成。
	float xSample = (x + m_rM->GetSeedX2()) / m_rM->GetRelief2();
	float ySample = (y + m_rM->GetSeedY2()) / m_rM->GetRelief2();
	float zSample = (z + m_rM->GetSeedZ2()) / m_rM->GetRelief2();

	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//ノイズが一定以内だったら木を生成。
	if (noise < minNoiseGenerateTree || noise > maxNoiseGenerateTree) {
		return false;
	}

	int xx = x;
	int yy = y + 1;
	int zz = z;
	noise *= noiseMultiply;
	int height = rand() * int(noise) % (maxLengthTreeTrunk - minLengthTreeTrunk + 1) + minLengthTreeTrunk;
	//木の幹をつくっていく。
	for (int i = 0; i < height; i++) {
		m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube_OakLog));
		yy += 1;
	}
	m_lengthTreeTrunk = yy;
	return true;
}

void TreeGenerator::GenerateLeaf(const int x, const int y, const int z)
{
	int xx = x;
	int yy = m_lengthTreeTrunk;
	int zz = z;

	yy -= rand() % minPositionLeaf + 1;

	int xxx = xx % numberTreeGenerationPatterns;
	int yyy = yy % numberTreeGenerationPatterns;
	int zzz = zz % numberTreeGenerationPatterns;

	float relief3 = m_rM->GetRelief3();

	float xSample = (xxx + seed) / relief3;
	float ySample = (yyy + seed) / relief3;
	float zSample = (zzz + seed) / relief3;

	//木の中心のノイズを求める。
	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//葉を生成していく。
	for (int i = 0; i < leafHeight; i++) {
		for (int j = -leafWidth + 1; j < leafWidth; j++) {
			for (int p = -leafDepth + 1; p < leafDepth; p++) {

				//既にブロックが存在していたら、葉は生成しないやで。
				if (m_world->GetBlock(xx + j, yy + i, zz + p) != nullptr)
					continue;

				//ノイズを生成していく。
				//一つのノイズだけだと偏るので。
				//複数のノイズを生成していく。
				int rm = rand() * int(noise * noiseMultiply2) % 2;
				int rm2 = rand() * int(noise * noiseMultiply2) % 2;

				float xSample = (xxx + j + seed + rm2) / relief3;
				float ySample = (yyy + i * valueAddYDeterminingLeafGeneration + seed) / relief3;
				float zSample = (zzz + p + seed + rm) / relief3;
				float noise2 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				//
				xSample = (xxx + j + seed + rm2) / relief3;
				ySample = (yyy + i * valueAddYDeterminingLeafGeneration + seed) / relief3;
				zSample = (zzz - p + seed - rm) / relief3;
				float noise3 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				xSample = (xxx - j + seed - rm2) / relief3;
				ySample = (yyy + i * valueAddYDeterminingLeafGeneration + seed) / relief3;
				zSample = (zzz + p + seed + rm) / relief3;
				float noise4 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				xSample = (xxx - j + seed - rm2) / relief3;
				ySample = (yyy + i * valueAddYDeterminingLeafGeneration + seed) / relief3;
				zSample = (zzz - p + seed - rm) / relief3;
				float noise5 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				//木の中心のノイズとの差が一定以内なら葉を生成する。
				if (abs(noise - noise2) > differenceNoiseGenerationLeaf ||
					abs(noise - noise3) > differenceNoiseGenerationLeaf ||
					abs(noise - noise4) > differenceNoiseGenerationLeaf ||
					abs(noise - noise5) > differenceNoiseGenerationLeaf)
					continue;
				m_world->SetBlock(xx + j, yy + i, zz + p, BlockFactory::CreateBlock(enCube_OakLeaf));
			}
		}
	}
}