#pragma once
#include "Entity.h"

struct GeneratorParameters
{
	//このmin～maxのブロック数だけプレイヤーから離れた所にモブを生成させる。
	//今のところ、xzのブロック数がどれだけ離れているか計算しています。
	//コードの都合上、max / 2 > min にしといてね。
	//24～128だとモブの生成がわかりづらいので範囲を狭めています。
	//const int minDistanceMobGenerate = 24;
	//const int maxDistanceMobGenerate = 128;
	const int minDistanceMobGenerate = 20;
	const int maxDistanceMobGenerate = 50;
	//設定した秒数後にモブの生成を開始する。
	const float mobGenerateTimer = 7.0f;
	//一度に生成するモブの最大数。
	const int maxNumberMobGenerateOneTime = 4;
	//範囲内に生成するモブの最大数。
	//範囲内というのは、プレイヤーからの距離が↑のmaxのブロック数以内のもの。
	const int maxNumberMobRange = 10;
	//全ワールドに生成するモブの最大数。
	const int maxNumberMob = 30;
};

class Player;
class World;
/// <summary>
/// モブを生成していくクラス。
/// モブはブロックのある場所にしか生成されません。
/// </summary>
class MobGenerator : public IGameObject
{
public:
	~MobGenerator() override;
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
protected:
	virtual void Init() = 0;
	
	/// <summary>
	/// 生成のパラメータを設定する。
	/// </summary>
	/// <param name="params"></param>
	void SetParametors(GeneratorParameters* params)
	{
		m_params = params;
	}
	/// <summary>
	/// 何のインスタンスを生成するかを設定する。
	/// </summary>
	/// <param name="typeEntity"></param>
	void SetTypeEntity(EnEntity typeEntity)
	{
		m_typeEntity = typeEntity;
	}
	/// <summary>
	/// モブを生成する。
	/// </summary>
	virtual void NewGOMob(const CVector3& pos) = 0;
	World* GetWorld()
	{
		return m_world;
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
	/// モブを生成する。
	/// </summary>
	void GenerateMob();
	/// <summary>
	/// モブが生成できる空間があるかどうかを検索。
	/// </summary>
	/// <param name="pos">ゾンビを生成予定の座標。</param>
	void SearchSpaceMobGenerate(CVector3& pos);
	/// <summary>
	/// モブが特定の範囲内にどれだけ存在しているか調べる。
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標。</param>
	void SearchNumberMobRange(CVector3& playerPos);
private:
	Player* m_player = nullptr;			//プレイヤー。
	World* m_world;				//ワールド。
	float m_timer = 0.0f;			//タイマー。
	int m_numberMobRange = 0;		//範囲内に存在しているモブ。
	int m_numberMob = 0;
	GeneratorParameters* m_params;
	EnEntity m_typeEntity;
};

