#pragma once

class World;
class Chunk;

/// <summary>
/// ���̊֌W
/// </summary>
class LightUtil
{
public:
	/// <summary>
	/// �`�d����
	/// </summary>
	/// <param name="world">���[���h</param>
	/// <param name="lightPower">���̋���</param>
	/// <param name="pos">���̈ʒu</param>
	/// <param name="fromDir">���̕���(�S���ʂ���)</param>
	/// <param name="isSkyLight">�X�J�C���C�g��?</param>
	static void SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight);

	//���邳���x���̐�
	static constexpr int LIGHT_POWER_COUNT = 16;
	//���邳���x���̍ő�l
	static constexpr char LIGHT_POWER_MAX = LIGHT_POWER_COUNT - 1;

	//�\����̖��邳
	//���̃��x����80%
	static constexpr float DRAWING_LIGHT[LIGHT_POWER_COUNT] = {
		0.035184372088832f,
		0.04398046511104f, 0.0549755813888f, 0.068719476736f, 0.08589934592f, 0.1073741824f,
		0.134217728f, 0.16777216f, 0.2097152f, 0.262144f, 0.32768f,
		0.4096f, 0.512f, 0.64f, 0.8f, 1.0f
	};

	//�����`���������
	static constexpr IntVector3 spreadDir[6] = {
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1},
	};
};

/// <summary>
/// �X�J�C���C�g�̌v�Z����
/// </summary>
class SkyLight
{
public:
	SkyLight(World* world) : m_world(world){}

	/// <summary>
	/// �X�J�C���C�g�̌v�Z���s��
	/// </summary>
	/// <param name="chunk">�v�Z����`�����N</param>
	void CalcSkyLight(Chunk* chunk);

private:
	World* m_world = nullptr;
};