#pragma once
#include"Entity.h"
#include"BlockType.h"
#include "ItemType.h"

//敵や動物に継承させるクラス。
class Mob : public Entity
{
public:

	Mob(EnEntity enEntity = enEntity_None);
	virtual ~Mob();

protected:
	void DroppingItem(CVector3 pos);
	virtual CVector3 GetPos()
	{
		return CVector3::Zero();
	}
	virtual void SetPos(const CVector3& pos)
	{

	}
	//アイテムをドロップする確率を設定する。
	void SetChanceDropping(int chanceDropping, bool isRare = false)
	{
		//アイテムをドロップさせる確率は1～100％。
		if (chanceDropping > 0 && chanceDropping < 101)
		{
			if (isRare) {
				m_rareChanceDropping = chanceDropping;
			}
			else {
				m_chanceDropping = chanceDropping;
			}
		}
	}
	//ドロップするアイテムを設定する。
	void SetDropItemId(int itemId, bool isRare = false)
	{
		if (itemId > enCube_None && itemId < enAllItem_Num)
		{
			if (isRare) {
				m_rareItemId = itemId;
			}
			else {
				m_itemId = itemId;
			}
		}
	}
	int m_itemId = enCube_None;		//ドロップするアイテムの種類、Noneだったらドロップしない。
	int m_chanceDropping = 0;		//アイテムをドロップさせる確率、1～100％。
	int m_rareItemId = enCube_None;
	int m_rareChanceDropping = 0;
};

