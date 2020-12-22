#include "stdafx.h"
#include "BiomeManager.h"
#include "Perlin.h"

BiomeManager::BiomeManager(){}

enBiome BiomeManager::DecideBiome(const int x,const int y,const int z)
{
	//�N���̌������B
	float relief = 70.0f;
	//�ō��C���B
	int maxTemp = 30;
	//�Œ�C���B
	int minTemp = -20;
	//�ō��~���ʁB
	int maxRain = 4000;
	//�Œ�~���ʁB
	int minRain = 0;

	//�m�C�Y���擾�B
	float noise = GetPerlin().OctavePerlin(x / relief + m_seedX, 0, z / relief + m_seedZ, 2);
	float noise2 = GetPerlin().OctavePerlin(x / relief + m_seedX2, 0, z / relief + m_seedZ2, 2);

	//�m�C�Y����C���ƍ~���ʂ����肷��B
	int temp = int(noise * (maxTemp - minTemp));
	temp += minTemp;
	int rain = int(noise2 * (maxRain - minRain));
	rain += minRain;

	enBiome state = enBiome_Forest;
	//�~����1500�ȏ�
	if (rain > 2000) {
		//�X�сB
		if (temp > 10) {
			state = enBiome_Forest;
		}
		/*//�����B
		else if (temp > -5) {
			state = enBiome_Plains;
		}
		//�c���h���B
		else {
			state = enBiome_Tundra;
		}*/
	}
	else {
		//�����B
		if (temp > 7) {
			state = enBiome_Desert;
		}
		//�c���h���B
		/*else {
			state = enBiome_Tundra;
		}*/
	}

	return state;
}

void BiomeManager::GenerateSeed( std::mt19937 & generater ){
	//�V�[�h�l������������B
	m_seedX = generater() % 101;
	m_seedZ = generater() % 101;

	m_seedX2 = m_seedX + generater() % 101 + 51;
	m_seedZ2 = m_seedZ + generater() % 101 + 51;
}
