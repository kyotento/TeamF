#pragma once


class World;
class RandomMapMaker;
/// <summary>
/// �؂𐶐�����N���X�B
/// </summary>
class TreeGenerator
{
public:
	void SetWorld(World* world)
	{
		m_world = world;
	}
	void SetRandomMapMaker(RandomMapMaker* rM)
	{
		m_rM = rM;
	}
	/// <summary>
	/// �؂𐶐����Ă����B
	/// </summary>
	/// <param name="rm">�����_���}�b�v���[�J�[�B</param>
	/// <param name="x">x���W�B</param>
	/// <param name="y">y���W�B</param>
	/// <param name="z">z���W�B</param>
	void GenerateTree(const int x, const int y, const int z);
private:
	/// <summary>
/// ���؂𐶐����Ă����B
/// </summary>
	bool GenerateWood(const int x, const int y, const int z);
	/// <summary>
	/// �t�𐶐����Ă����B
	/// </summary>
	void GenerateLeaf(const int x, const int y, const int z);

private:
	World* m_world = nullptr;
	RandomMapMaker* m_rM = nullptr;
	int m_lengthTreeTrunk = 0;
};

