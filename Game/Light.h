#pragma once

class World;
class Chunk;

/// <summary>
/// 光の関係
/// </summary>
class LightUtil
{
public:
	/// <summary>
	/// 伝播処理
	/// </summary>
	/// <param name="world">ワールド</param>
	/// <param name="lightPower">光の強さ</param>
	/// <param name="pos">光の位置</param>
	/// <param name="fromDir">光の方向(全方位あり)</param>
	/// <param name="isSkyLight">スカイライトか?</param>
	static void SpreadLight(World* world, char lightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight);

	//明るさレベルの数
	static constexpr int LIGHT_POWER_COUNT = 16;
	//明るさレベルの最大値
	static constexpr char LIGHT_POWER_MAX = LIGHT_POWER_COUNT - 1;

	//表示上の明るさ
	//一つ上のレベルの80%
	static constexpr float DRAWING_LIGHT[LIGHT_POWER_COUNT] = {
		0.035184372088832f,
		0.04398046511104f, 0.0549755813888f, 0.068719476736f, 0.08589934592f, 0.1073741824f,
		0.134217728f, 0.16777216f, 0.2097152f, 0.262144f, 0.32768f,
		0.4096f, 0.512f, 0.64f, 0.8f, 1.0f
	};

	//光が伝搬する方向
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
/// スカイライトの計算する
/// </summary>
class SkyLight
{
public:
	SkyLight(World* world) : m_world(world){}

	/// <summary>
	/// スカイライトの計算を行う
	/// </summary>
	/// <param name="chunk">計算するチャンク</param>
	void CalcSkyLight(Chunk* chunk);

private:
	World* m_world = nullptr;
};