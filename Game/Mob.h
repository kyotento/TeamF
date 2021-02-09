#pragma once
#include"Entity.h"
#include"BlockType.h"
#include "ItemType.h"

//�G�⓮���Ɍp��������N���X�B
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
	//�A�C�e�����h���b�v����m����ݒ肷��B
	void SetChanceDropping(int chanceDropping, bool isRare = false)
	{
		//�A�C�e�����h���b�v������m����1�`100���B
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
	//�h���b�v����A�C�e����ݒ肷��B
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
	int m_itemId = enCube_None;		//�h���b�v����A�C�e���̎�ށANone��������h���b�v���Ȃ��B
	int m_chanceDropping = 0;		//�A�C�e�����h���b�v������m���A1�`100���B
	int m_rareItemId = enCube_None;
	int m_rareChanceDropping = 0;
};

