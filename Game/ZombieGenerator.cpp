#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Player.h"
#include "World.h"
#include "Zombie.h"

namespace {
	//このmin～maxのブロック数だけプレイヤーから離れた所にゾンビを生成させる。
	//今のところ、xzのブロック数がどれだけ離れているか計算しています。
	//コードの都合上、max / 2 > min にしといてね。
	//24～128だとゾンビの生成がわかりづらいので範囲を狭めています。
	//const int minDistanceZombieGenerate = 24;
	//const int maxDistanceZombieGenerate = 128;
	const int minDistanceZombieGenerate = 20;
	const int maxDistanceZombieGenerate = 50;
	//設定した秒数後にゾンビの生成を開始する。
	const float zombieGenerateTimer = 7.0f;
	//一度に生成するゾンビの最大数。
	const int maxNumberZombieGenerateOneTime = 4;
	//範囲内に生成するゾンビの最大数。
	//範囲内というのは、プレイヤーからの距離が↑のmaxのブロック数以内のもの。
	const int maxNumberZombieRange = 10;
	//全ワールドに生成するゾンビの最大数。
	const int maxNumberZombie = 30;
	//乱数生成。
	std::mt19937 random((std::random_device())());
}

bool ZombieGenerator::Start()
{
	return true;
}

void ZombieGenerator::Update()
{
	if (GetPlayer() == nullptr)
	{
		return;
	}
	//ゾンビ生成。
	GenerateZombie();
	//タイマー加算。
	m_timer += GetEngine().GetRealDeltaTimeSec();
}

void ZombieGenerator::GenerateZombie()
{
	//一定時間経っていたら、ゾンビ生成。
	if (m_timer >= zombieGenerateTimer)
	{
		CVector3 playerPos = m_player->GetPos() / Block::WIDTH;
		m_numberZombieRange = 0;
		m_numberZombie = 0;
		//範囲内にいるゾンビの数を調べる。
		SearchNumberZombieRange(playerPos);
		//ゾンビを生成したい回数分。
		for (int i = 0; i < maxNumberZombieGenerateOneTime; i++)
		{
			if (m_numberZombieRange >= maxNumberZombieRange || m_numberZombie >= maxNumberZombie)
			{
				break;
			}
			//特定の範囲内の乱数を生成、それぞれxとz。
			//-max / 2 ～ -min / 2 と min / 2 ～ max / 2 の範囲の乱数。
			int ranX = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate / 2 + 1) + minDistanceZombieGenerate / 2;
			int ranZ = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate / 2 + 1) + minDistanceZombieGenerate / 2;
			//乱数で余り0だったら乱数を－にする。
			if (random() % 2)
			{
				ranX = -ranX;
			}
			if (random() % 2)
			{
				ranZ = -ranZ;
			}
			CVector3 pos = CVector3(playerPos.x + ranX, 0.0f, playerPos.z + ranZ);
			//ゾンビが生成できるスペースがあったらゾンビを生成していく。
			SearchSpaceZombieGenerate(pos);
		}
		//タイマーをリセット。
		m_timer = 0.0f;
	}
}

void ZombieGenerator::SearchSpaceZombieGenerate(CVector3& pos)
{
	//下からブロックが存在するかどうかを見ていく。
	//上からだと木の上にゾンビが生成されるので上からにしました。
	for (int i = 0; i < Chunk::HEIGHT - 2; i++)
	{
		//ブロックが存在していたら。
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {
			//上2マスに空きが存在しているかどうか調べる。
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				//空きがあったらゾンビ生成。
				Zombie* zombie = m_world->CreateEntity<Zombie>();
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				m_numberZombieRange++;
				m_numberZombie++;
				return;
			}
		}
	}
}

void ZombieGenerator::SearchNumberZombieRange(CVector3& playerPos)
{
	//ワールドからエンティティの配列持ってくる。
	std::unordered_set<Entity*> entities = m_world->GetEntities();
	for (auto entity : entities)
	{
		//エンティティのタイプがゾンビだったら。
		if (entity->GetEntityType() == enEntity_Zombie)
		{
			m_numberZombie++;
			CVector3 zombiePos = entity->GetPos() / Block::WIDTH;
			CVector3 diff = playerPos - zombiePos;
			//プレイヤーとの距離が一定範囲内だったら。
			if (diff.LengthSq() <= maxDistanceZombieGenerate * maxDistanceZombieGenerate)
			{
				//範囲内のゾンビとしてカウントする。
				m_numberZombieRange++;
			}
		}
	}
}