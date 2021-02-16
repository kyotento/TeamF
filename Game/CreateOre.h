#pragma once
#include "BlockType.h"
class World;
class RandomMapMaker;
//�z�Όn�������̂��߂̃f�[�^
struct OreData {
	//�ő�̍���
	int s_oreMaxHeight = 0;
	//�ŏ��̍���
	int s_oreMinHeight = 0;
	//���̍����̍���
	int s_oreDiffHeight = 0;
	//�ő吔
	int s_oreMaxNumber = 0;
	//�����_�ɐ��������z�΂̐�
	int s_oreNumber = 0;
	//��̑傫��
	int s_oreWidth = 0;
	int s_oreDepth = 0;
	int s_oreHeight = 0;
	//���������p�[�Z���e�[�W
	int s_prob = 0;
	//����������
	int s_oreDivideRand = 1;
	//�����l(�ΒY�p)
	int s_atten = 0;
	//����ɐ������鎞�Ɏ΂߂ɐ������邩�H
	bool s_isDig = true;
};

//�p�[�����m�C�Y�ł�낤�Ƃ����f�[�^
struct PerlinData {
	//�ő�̍���
	int s_perlinMaxHeight = 0;
	//�ŏ��̍���
	int s_perlinMinHeight = 0;
	//�ő吔
	int s_perlinMaxNumber = 0;
	//���������z�΂̐�
	int s_perlinNumber = 0;
	//�ŏ��̃m�C�Y�l
	float s_minNoise = 0.f;
	//�ő�̃m�C�Y�l
	float s_maxNoise = 0.f;
	//�N���̌�����
	float s_relief = 0.0f;
};

class Chunk;

class CreateOre
{
public:
	//�z�΂𐶐����邼
	void Awake(Chunk& chunk);
	//���[���h��ݒ�
	void SetWorld(World* world)
	{
		m_world = world;
	}
	//�}�b�v���[�J�[��ݒ�
	void SetMapMaker(RandomMapMaker* mapMaker)
	{
		m_mapMaker = mapMaker;
	}
private:
	//�����_�ɍz�΂𐶐����邩�ǂ������f����
	void DetermineGenerate(const int x, const int y, const int z);
	//�����_�̎���ɍz�΂𐶐�����
	void CreateAround(const int x, const int y, const int z, EnCube enCube, bool isDig,int p = 0);
	//���͎g���ĂȂ��p�[�����m�C�Y�Ő�������
	void GeneratePerlinNoise(const int x, const int y, const int z);
private:
	World* m_world = nullptr;			//���[��
	RandomMapMaker* m_mapMaker = nullptr;		//�}�b�v���[�J�[
	std::map<EnCube, OreData> m_oreDataList;
	std::map<EnCube, PerlinData> m_perlinDataList;
	int m_seedX, m_seedY, m_seedZ = 0;
	int m_width, m_depth, m_maxHeight, m_minHeight;			//�}�b�v�̍L���B
	int m_startX, m_startZ;    //�}�b�v�̃X�^�[�g�l�B
};

