#pragma once
#include "Block.h"

/*
/// <summary>
/// 原木ブロック
/// 破壊されたときに葉を破壊する
/// </summary>
class WoodLog : public Block
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="woodType">原木の種類</param>
	/// <param name="leafType">葉の種類</param>
	WoodLog(EnCube woodType, EnCube leafType) : m_woodType(woodType), m_leafType(leafType) {}

	~WoodLog() {
		DestoroyLeaf();
	}

private:
	void DestoroyLeaf();
	[[nodiscard]] bool DestoroyLeafInner(const IntVector3& pos, const IntVector3& fromDir, int floor, std::list<Block*>& leaflist);

	EnCube m_woodType;
	EnCube m_leafType;
};
*/

class Leaf : public Block, public IGameObject {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="woodType">原木の種類</param>
	/// <param name="leafType">葉の種類</param>
	Leaf(EnCube woodType, EnCube leafType) : m_woodType(woodType), m_leafType(leafType)
	{
		m_cnt = REFLEASH_COUNT / 20 * CMath::IntUniDist(20);
	}

private:
	void Update()override;

	[[nodiscard]] bool FindLog(const IntVector3& pos, const IntVector3& fromDir, int floor);

	EnCube m_woodType;
	EnCube m_leafType;

	int m_cnt = 0;
	//int m_leafHp = 6;

	static constexpr int REFLEASH_COUNT = 60 * 10;//更新間隔
};

