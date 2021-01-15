#include "stdafx.h"
#include "MobGenerator.h"
#include "Player.h"
#include "World.h"

namespace {
	//乱数生成。
	std::mt19937 random((std::random_device())());
}

bool MobGenerator::Start()
{
	Init();
	return true;
}

void MobGenerator::Update()
{
	if (GetPlayer() == nullptr)
	{
		return;
	}
	//モブ生成。
	GenerateMob();
	//タイマー加算。
	m_timer += GetEngine().GetRealDeltaTimeSec();
}

void MobGenerator::GenerateMob()
{
	//一定時間経っていたら、モブ生成。
	if (m_timer >= m_params->mobGenerateTimer)
	{
		CVector3 playerPos = m_player->GetPos() / Block::WIDTH;
		m_numberMobRange = 0;
		m_numberMob = 0;
		//範囲内にいるモブの数を調べる。
		SearchNumberMobRange(playerPos);
		//モブを生成したい回数分。
		for (int i = 0; i < m_params->maxNumberMobGenerateOneTime; i++)
		{
			if (m_numberMobRange >= m_params->maxNumberMobRange || m_numberMob >= m_params->maxNumberMob)
			{
				break;
			}
			//特定の範囲内の乱数を生成、それぞれxとz。
			//-max / 2 ～ -min / 2 と min / 2 ～ max / 2 の範囲の乱数。
			int ranX = random() % (m_params->maxDistanceMobGenerate / 2 - m_params->minDistanceMobGenerate / 2 + 1) + m_params->minDistanceMobGenerate / 2;
			int ranZ = random() % (m_params->maxDistanceMobGenerate / 2 - m_params->minDistanceMobGenerate / 2 + 1) + m_params->minDistanceMobGenerate / 2;
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
			//モブが生成できるスペースがあったらモブを生成していく。
			SearchSpaceMobGenerate(pos);
		}
		//タイマーをリセット。
		m_timer = 0.0f;
	}
}

void MobGenerator::SearchSpaceMobGenerate(CVector3& pos)
{
	//下からブロックが存在するかどうかを見ていく。
	//上からだと木の上にモブが生成されるので上からにしました。
	for (int i = 0; i < Chunk::HEIGHT - 2; i++)
	{
		//ブロックが存在していたら。
		if (m_world->GetBlock(CVector3(pos.x, i, pos.z)) != nullptr) {
			//上2マスに空きが存在しているかどうか調べる。
			if (m_world->GetBlock(CVector3(pos.x, i + 1, pos.z)) == nullptr &&
				m_world->GetBlock(CVector3(pos.x, i + 2, pos.z)) == nullptr)
			{
				//空きがあったらモブ生成。
				/*Zombie* zombie = NewGO<Zombie>(m_world);
				zombie->SetPos(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));*/
				NewGOMob(CVector3(pos.x * Block::WIDTH, (float(i) + 1.0f) * Block::WIDTH, pos.z * Block::WIDTH));
				m_numberMobRange++;
				m_numberMob++;
				return;
			}
		}
	}
}

void MobGenerator::SearchNumberMobRange(CVector3& playerPos)
{
	//ワールドからエンティティの配列持ってくる。
	std::unordered_set<Entity*> entities = m_world->GetEntities();
	for (auto entity : entities)
	{
		//エンティティのタイプがモブだったら。
		if (entity->GetEntityType() == m_typeEntity)
		{
			m_numberMob++;
			CVector3 mobPos = entity->GetPos() / Block::WIDTH;
			CVector3 diff = playerPos - mobPos;
			//プレイヤーとの距離が一定範囲内だったら。
			if (diff.LengthSq() <= m_params->maxDistanceMobGenerate * m_params->maxDistanceMobGenerate)
			{
				//範囲内のモブとしてカウントする。
				m_numberMobRange++;
			}
		}
	}
}