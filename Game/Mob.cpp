#include "stdafx.h"
#include "Mob.h"
#include "DropItem.h"
#include "Block.h"

namespace {
	const int randomDrop = Block::WIDTH / 1.5f;	//��񂿂イ�̂͂񂢁B
	std::mt19937 random((std::random_device())());	//��񂿂イ�B
}

Mob::Mob(EnEntity enEntity) : Entity(enEntity_Cow)
{
}

void Mob::DroppingItem(CVector3 pos)
{
	//�h���b�v����A�C�e�����ݒ肳��Ă��邩�A�h���b�v����m����0���ゾ�����ꍇ�B
	//if (m_itemId != enCube_None && m_chanceDropping > 0)
	{
		int rand = random() % 101;
		//if (rand < m_chanceDropping) 
		{
			//�h���b�v�A�C�e�����쐬�B
			DropItem* dropItem = DropItem::CreateDropItem(m_world, m_itemId);
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
