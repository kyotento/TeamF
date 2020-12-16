#include "stdafx.h"
#include "ZombieGenerator.h"
#include "Player.h"
#include "World.h"
#include "Zombie.h"

namespace {
	//このmin～maxのブロック数だけプレイヤーから離れた所にゾンビを生成させる。
	//今のところ、xzのブロック数がどれだけ離れているか計算しています。
	//コードの都合上、max / 2 > min にしといてね。
	const int minDistanceZombieGenerate = 24;
	const int maxDistanceZombieGenerate = 128;
	//設定した秒数後にゾンビの生成を開始する。
	const float zombieGenerateTimer = 10.0f;
	//一度に生成するゾンビの最大数。
	const int maxNumberZombieGenerate = 3;
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
		//ゾンビを生成したい回数分。
		for (int i = 0; i < maxNumberZombieGenerate; i++)
		{
			//特定の範囲内の乱数を生成、それぞれxとy。
			int ranX = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate + 1) + minDistanceZombieGenerate;
			int ranZ = random() % (maxDistanceZombieGenerate / 2 - minDistanceZombieGenerate + 1) + minDistanceZombieGenerate;
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
			SearchSpaceZombieGenerate(pos);
		}
		//タイマーをリセット。
		m_timer = 0.0f;
	}
}

void ZombieGenerator::SearchSpaceZombieGenerate(CVector3& pos)
{
	//上からブロックが存在するかどうかを見ていく。
	for (int i = Chunk::HEIGHT; i > 0; i--)
	{
		//ブロックが存在していたら。
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {

			//上2マスに空きが存在しているかどうか調べる。
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				Enemy* zombie = NewGO<Zombie>(m_world);
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				break;
			}
		}


	}
}