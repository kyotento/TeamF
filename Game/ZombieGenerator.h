#pragma once

class Player;
class World;
/// <summary>
/// ゾンビを生成していくクラス。
/// ゾンビはブロックのある場所にしか生成されません。
/// </summary>
class ZombieGenerator : public IGameObject
{
public:
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ワールドをセットする。
	/// </summary>
	/// <param name="world">ワールド。</param>
	void SetWorld(World* world)
	{
		m_world = world;
	}
private:
	Player* GetPlayer()
	{
		if (m_player == nullptr)
		{
			m_player = FindGO<Player>(L"player");
		}
		return m_player;
	}
	/// <summary>
	/// ゾンビを生成する。
	/// </summary>
	void GenerateZombie();
	/// <summary>
	/// ゾンビが生成できる空間があるかどうかを検索。
	/// </summary>
	/// <param name="pos">ゾンビを生成予定の座標。</param>
	void SearchSpaceZombieGenerate(CVector3& pos);
	/// <summary>
	/// ゾンビが特定の範囲内にどれだけ存在しているか調べる。
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標。</param>
	void SearchNumberZombieRange(CVector3& playerPos);
private:
	Player* m_player = nullptr;			//プレイヤー。
	World* m_world;				//ワールド。
	float m_timer = 0.0f;			//タイマー。
	int m_numberZombieRange = 0;		//範囲内に存在しているゾンビ。
};

