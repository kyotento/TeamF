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

	/// <summary>
	/// ���̓`�d����
	/// </summary>
	/// <param name="world">���[���h</param>
	/// <param name="lightPower">�ȑO�̌��̋���</param>
	/// <param name="pos">���̈ʒu</param>
	/// <param name="fromDir">���̕���(�S���ʂ���)</param>
	/// <param name="isSkyLight">�X�J�C���C�g��?</param>
	static int SpreadDark(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight);
	static void SpreadDarkInner(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight, std::list<std::unique_ptr<IntVector3>>& refleshList, int nest);

	//���邳���x���̐�
	static constexpr int LIGHT_POWER_COUNT = 16;
	//���邳���x���̍ő�l
	static constexpr char LIGHT_POWER_MAX = LIGHT_POWER_COUNT - 1;

	//�\����̖��邳
	//���̃��x����80%
	static constexpr float DRAWING_LIGHT[LIGHT_POWER_COUNT] = {
		0.035f,
		0.043f, 0.054f, 0.068f, 0.085f, 0.107f,
		0.134f, 0.167f, 0.209f, 0.262f, 0.327f,
		0.4096f, 0.512f, 0.64f, 0.8f, 1.0f
	};
	static constexpr float DRAWING_SKY_LIGHT[LIGHT_POWER_COUNT] = {
		0.035f,
		0.043f, 0.054f, 0.068f, 0.085f, 0.107f,
		0.134f, 0.167f, 0.209f, 0.262f, 0.327f,
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

	/// <summary>
	/// �w��̈ʒu�̃X�J�C���C�g�v�Z
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="isBlock"></param>
	void CalcSkyLightThisPosition(IntVector3 pos, bool isBlock);

private:
	World* m_world = nullptr;
};