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

	//�V�[�h�l��������
	m_seedX = random() % seed;
	m_seedY = random() % seed;
	m_seedZ = random() % seed;

	//�}�b�v�̍L�������Ă���

	m_maxHeight = m_mapMaker->m_maxHeight;
	m_minHeight = m_mapMaker->m_minHeight;
	m_startX = chunk.CalcWorldCoordX(0);
	m_startZ = chunk.CalcWorldCoordZ(0);
	m_width = chunk.CalcWorldCoordX(Chunk::WIDTH);
	m_depth = chunk.CalcWorldCoordZ(Chunk::WIDTH);

	//�z�΂̃f�[�^��������
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

	//���������z�΂̌����v��
	int oreNumber = 0;
	for (int x = m_startX; x < m_width; x++) {
		for (int z = m_startZ; z < m_depth; z++) {
			for (int y = m_maxHeight; y >= m_minHeight; y--) {
				//(��,_�T��)�z�΂𐶐����邩�ǂ������f�����ł��킟�E�E�E
				DetermineGenerate(x, y, z);
			}
		}
	}
}

void CreateOre::DetermineGenerate(const int x, const int y, const int z)
{
	//�f�[�^�̃��X�g���܂킵��
	for (auto& itr : m_oreDataList) {
		//�����������ĂȂ������A���邢�͈��ȏ㐶�����Ă�����ȉ��������Ȃ�
		if (itr.second.s_oreMinHeight >= y || y >= itr.second.s_oreMaxHeight || itr.second.s_oreNumber >= itr.second.s_oreMaxNumber)
			continue;
		
		//0�Ŋ���ƃG���[�N����̂ŃX���[
		if (itr.second.s_oreDivideRand == 0)
			continue;
		//�����𐶐����āB
		int r = random() % itr.second.s_oreDivideRand;
	
		//�������͈͓���������z�ΐ���
		if (r <= itr.second.s_oreMaxNumber) {
			m_world->SetBlock(x, y, z, BlockFactory::CreateBlock(itr.first));
			itr.second.s_oreNumber++;
			//����ɂ��z�ΐ���
			CreateAround(x, y, z, itr.first, itr.second.s_isDig, itr.second.s_prob); 
			return;
		}
	}
}

void CreateOre::GeneratePerlinNoise(const int x, const int y, const int z)
{
	//�f�[�^�̃��X�g���܂킵��
	for (auto& itr : m_perlinDataList) {
		//�����������ĂȂ������A���邢�͈��ȏ㐶�����Ă�����ȉ��������Ȃ�
		if (itr.second.s_perlinMinHeight >= y || y >= itr.second.s_perlinMaxHeight || itr.second.s_perlinNumber >= itr.second.s_perlinMaxNumber)
			continue;
		//�m�C�Y�𐶐�����
		float xSample = (x + m_seedX) / itr.second.s_relief;
		float ySample = (y + m_seedY) / itr.second.s_relief;
		float zSample = (z + m_seedZ) / itr.second.s_relief;

		float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);
		//�m�C�Y�����ȓ��Ȃ琶������
		if (itr.second.s_minNoise <= noise && noise <= itr.second.s_maxNoise) {
			//m_world->SetBlock(x, y, z, BlockFactory::CreateBlock(itr.first), );
			itr.second.s_perlinNumber++;
		}
	}
}

void CreateOre::CreateAround(const int x, const int y, const int z, EnCube enCube, bool isDig, int p)
{
	//�΂߂ɐ������Ȃ�
	if (!isDig) {
		std::vector<CVector3> posList;
		posList.push_back(CVector3(-1, 0, 0));
		posList.push_back(CVector3(1, 0, 0));
		posList.push_back(CVector3(0, -1, 0));
		posList.push_back(CVector3(0, 1, 0));
		posList.push_back(CVector3(0, 0, -1));	
		posList.push_back(CVector3(0, 0, 1));

		//������ƃR�����g�����̂߂�ǂ�
		for (int i = 0; i < posList.size(); i++) {
			int xx = x + int(posList[i].x);
			int yy = y + int(posList[i].y);
			int zz = z + int(posList[i].z);
			if (m_width < xx || m_maxHeight < yy || yy < m_minHeight || m_depth < zz)
				continue;
			int r = random() % 100;
			//�������͈͂Ȃ��Ȃ琶��
			if (r < p) {
				m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube));
				if (enCube == enCube_Soil) {
					int prob = p - m_oreDataList[enCube].s_atten;
					if (prob <= 0) {
						continue;
					}
					//�X�Ɏ���ɂ��������Ă���
					CreateAround(xx, yy, zz, enCube, m_oreDataList[enCube].s_isDig, prob);
				}
			}
		}
		return;
	}

	//�΂߂ɐ�������
	for (int xx = x; xx < x + m_oreDataList[enCube].s_oreWidth; xx++) {
		for (int yy = y; yy < y + m_oreDataList[enCube].s_oreHeight; yy++) {
			for (int zz = z; zz < z + m_oreDataList[enCube].s_oreDepth; zz++) {
				if (m_width < xx || m_maxHeight < yy || yy < m_minHeight || m_depth < zz)
					continue;
				int r = random() % 100;
				//�������͈͓��Ȃ琶��
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