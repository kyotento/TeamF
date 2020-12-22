#pragma once


class World;
class RandomMapMaker;
/// <summary>
/// 木を生成するクラス。
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
	/// 木を生成していく。
	/// </summary>
	/// <param name="rm">ランダムマップメーカー。</param>
	/// <param name="x">x座標。</param>
	/// <param name="y">y座標。</param>
	/// <param name="z">z座標。</param>
	void GenerateTree(const int x, const int y, const int z);
private:
	/// <summary>
/// 原木を生成していく。
/// </summary>
	bool GenerateWood(const int x, const int y, const int z);
	/// <summary>
	/// 葉を生成していく。
	/// </summary>
	void GenerateLeaf(const int x, const int y, const int z);

private:
	World* m_world = nullptr;
	RandomMapMaker* m_rM = nullptr;
	int m_lengthTreeTrunk = 0;
};

