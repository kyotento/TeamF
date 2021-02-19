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

	/// <summary>
	/// 負の伝播処理
	/// </summary>
	/// <param name="world">ワールド</param>
	/// <param name="lightPower">以前の光の強さ</param>
	/// <param name="pos">光の位置</param>
	/// <param name="fromDir">光の方向(全方位あり)</param>
	/// <param name="isSkyLight">スカイライトか?</param>
	static int SpreadDark(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight);
	static void SpreadDarkInner(World* world, char oldLightPower, const IntVector3& pos, const IntVector3& fromDir, bool isSkyLight, std::list<std::unique_ptr<IntVector3>>& refleshList, int nest);

	//明るさレベルの数
	static constexpr int LIGHT_POWER_COUNT = 16;
	//明るさレベルの最大値
	static constexpr char LIGHT_POWER_MAX = LIGHT_POWER_COUNT - 1;

	//表示上の明るさ
	//一つ上のレベルの80%
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

	/// <summary>
	/// 指定の位置のスカイライト計算
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="isBlock"></param>
	void CalcSkyLightThisPosition(IntVector3 pos, bool isBlock);

private:
	World* m_world = nullptr;
};