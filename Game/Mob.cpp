#include "stdafx.h"
#include "Mob.h"
#include "DropItem.h"
#include "Block.h"

namespace {
	const int randomDrop = Block::WIDTH / 1.5f;	//��񂿂イ�̂͂񂢁B
	std::mt19937 random((std::random_device())());	//��񂿂イ�B
}

Mob::Mob(EnEntity enEntity) : Entity(enEntity)
{
}

void Mob::DroppingItem(CVector3 pos)
{
	//�h���b�v����A�C�e�����ݒ肳��Ă��邩�A�h���b�v����m����0���ゾ�����ꍇ�B
	if (m_itemId != enCube_None && m_chanceDropping > 0)
	{
		int rand = random() % 100;
		int dropItemId = enCube_None;

		if (m_rareItemId != enCube_None && rand < m_rareChanceDropping)
		{
			//���A�h���b�v
			dropItemId = m_rareItemId;
		}
		else if (rand < m_chanceDropping)
		{
			dropItemId = m_itemId;
		}

		if (dropItemId != enCube_None) {
			//�h���b�v�A�C�e�����쐬�B
			DropItem* dropItem = DropItem::CreateDropItem(m_world, dropItemId);
			CVector3 addPos = CVector3::Zero();
			if (random() % 2 > 0) {
				addPos.x += random() % randomDrop;
			}
			else {
				addPos.x -= random() % randomDrop;
			}

			if (random() % 2 > 0) {
				addPos.z += random() % randomDrop;
			}
			else {
				addPos.z += random() % randomDrop;
			}
			CVector3 position = pos;
			dropItem->SetPos(CVector3(position.x, position.y - 40.0f, position.z) + addPos);
		}
	}
}

Mob::~Mob()
{
	
}
