#include "stdafx.h"
#include "TreeGenerator.h"
#include "BlockFactory.h"
#include "Perlin.h"
#include "World.h"
#include "RandomMapMaker.h"

namespace {
	//��������؂̊��Ɋւ���ϐ��B
	//�p�[�����m�C�Y�Ńm�C�Y�𐶐����Ă���min�`max�܂ł̒l��������B
	//�؂𐶐����܂��B
	float minNoiseGenerateTree = 0.6f;
	float maxNoiseGenerateTree = 0.607f;
	//�؂̌��؂̍ŏ��̒����ƍő�̒����B
	int minLengthTreeTrunk = 5;
	int maxLengthTreeTrunk = 7;

	//��������t�̐����Ɋւ���ϐ��B
	//�ǂ�����t�𐶂₷���A��ԏ�̖؂̊�����ǂꂾ�����ɂ������B
	int minPositionLeaf = 3;
	//�t�̐����p�^�[���A��������Ηt�̌`��������B
	//���̐��l��3�悪�t�̐����p�^�[���ɂȂ�A�����B
	int numberTreeGenerationPatterns = 12;
	//�t�����������͈͂̃p�����[�^�B
	//�t�̍����B
	int leafHeight = 4;
	//�t�̍L���B
	int leafWidth = 3;
	int leafDepth = 3;
	//���̒l��傫��������t����葽�����������悤�ɂȂ�B
	float differenceNoiseGenerationLeaf = 0.0015f;
	//�t�𐶐��𔻒f����ۂɁA�m�C�Y�����̎���Y���W�ɉ��Z����l�B
	//���̒l��傫������ƃm�C�Y���傫���ϓ����AY���W�ɗt����������ɂ����Ȃ�B
	int valueAddYDeterminingLeafGeneration = 15;

	//��������͘M��Ȃ��������������B
	//�m�C�Y�ɏ�Z�B
	float noiseMultiply = 100.f;
	int noiseMultiply2 = 10000;
	float seed = 13;
}

void TreeGenerator::GenerateTree(const int x, const int y, const int z)
{
	//�؂��������ꂽ��B
	if (GenerateWood(x, y, z))
	{
		//�t���������Ă����B
		GenerateLeaf(x, y, z);
	}
}

bool TreeGenerator::GenerateWood(const int x, const int y, const int z)
{
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h�����B
	float xSample = (x + m_rM->GetSeedX2()) / m_rM->GetRelief2();
	float ySample = (y + m_rM->GetSeedY2()) / m_rM->GetRelief2();
	float zSample = (z + m_rM->GetSeedZ2()) / m_rM->GetRelief2();

	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//�m�C�Y�����ȓ���������؂𐶐��B
	if (noise < minNoiseGenerateTree || noise > maxNoiseGenerateTree) {
		return false;
	}

	int xx = x;
	int yy = y + 1;
	int zz = z;
	noise *= noiseMultiply;
	int height = rand() * int(noise) % (maxLengthTreeTrunk - minLengthTreeTrunk + 1) + minLengthTreeTrunk;
	//�؂̊��������Ă����B
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

	//�؂̒��S�̃m�C�Y�����߂�B
	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//�t�𐶐����Ă����B
	for (int i = 0; i < leafHeight; i++) {
		for (int j = -leafWidth + 1; j < leafWidth; j++) {
			for (int p = -leafDepth + 1; p < leafDepth; p++) {

				//���Ƀu���b�N�����݂��Ă�����A�t�͐������Ȃ���ŁB
				if (m_world->GetBlock(xx + j, yy + i, zz + p) != nullptr)
					continue;

				//�m�C�Y�𐶐����Ă����B
				//��̃m�C�Y�������ƕ΂�̂ŁB
				//�����̃m�C�Y�𐶐����Ă����B
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

				//�؂̒��S�̃m�C�Y�Ƃ̍������ȓ��Ȃ�t�𐶐�����B
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