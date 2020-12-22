#include "stdafx.h"
#include "BiomeManager.h"
#include "Perlin.h"

BiomeManager::BiomeManager(){}

enBiome BiomeManager::DecideBiome(const int x,const int y,const int z)
{
	//起伏の激しさ。
	float relief = 70.0f;
	//最高気温。
	int maxTemp = 30;
	//最低気温。
	int minTemp = -20;
	//最高降水量。
	int maxRain = 4000;
	//最低降水量。
	int minRain = 0;

	//ノイズを取得。
	float noise = GetPerlin().OctavePerlin(x / relief + m_seedX, 0, z / relief + m_seedZ, 2);
	float noise2 = GetPerlin().OctavePerlin(x / relief + m_seedX2, 0, z / relief + m_seedZ2, 2);

	//ノイズから気温と降水量を決定する。
	int temp = int(noise * (maxTemp - minTemp));
	temp += minTemp;
	int rain = int(noise2 * (maxRain - minRain));
	rain += minRain;

	enBiome state = enBiome_Forest;
	//降水量1500以上
	if (rain > 2000) {
		//森林。
		if (temp > 10) {
			state = enBiome_Forest;
		}
		/*//平原。
		else if (temp > -5) {
			state = enBiome_Plains;
		}
		//ツンドラ。
		else {
			state = enBiome_Tundra;
		}*/
	}
	else {
		//砂漠。
		if (temp > 7) {
			state = enBiome_Desert;
		}
		//ツンドラ。
		/*else {
			state = enBiome_Tundra;
		}*/
	}

	return state;
}

void BiomeManager::GenerateSeed( std::mt19937 & generater ){
	//シード値を初期化する。
	m_seedX = generater() % 101;
	m_seedZ = generater() % 101;

	m_seedX2 = m_seedX + generater() % 101 + 51;
	m_seedZ2 = m_seedZ + generater() % 101 + 51;
}
