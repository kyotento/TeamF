#pragma once
#include "Player.h"

class Box : public IGameObject
{
public:
	Box();
	~Box() {}
	void PostRender() override;
public:
	int AddItem(EnCube enCube, int number);
private:
	//�C���x���g���̒���
	static const int m_inventryWidth = 9;
	std::vector<Inventory*>  m_inventoryList;
	CFont m_font;
};

